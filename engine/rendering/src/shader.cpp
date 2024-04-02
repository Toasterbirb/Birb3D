#include "Globals.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "ShaderSource.hpp"

#include <array>
#include <cassert>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace birb
{
	// Cache for compiled shaders
	static std::unordered_map<std::string, u32> shader_cache;
	static size_t shader_cache_hit_count = 0;

	shader::shader()
	{}

	shader::shader(const std::string& shader_name)
	:vertex_shader_name(shader_name), fragment_shader_name(shader_name)
	{
		static_assert(point_light_count > 0);
		static_assert(point_light_count < 8192, "Arbitrary limitation, but you are probably doing something wrong");

		compile_shader(shader_name, shader_name);
		assert(id != 0);
	}

	shader::shader(const std::string& vertex, const std::string& fragment)
	:vertex_shader_name(vertex), fragment_shader_name(fragment)
	{
		compile_shader(vertex, fragment);
		assert(id != 0);
	}

	shader::shader(const shader& other)
	{
		PROFILER_SCOPE_MISC("Shader copy")

		birb::log("Shader copy causing a recompile (" + birb::ptr_to_str(&other) + " -> " + birb::ptr_to_str(this) + ")");

		id = 0;
		vertex_shader_name		= other.vertex_shader_name;
		fragment_shader_name	= other.fragment_shader_name;

		compile_shader(other.vertex_shader_name, other.fragment_shader_name);

		// Add all uniform locations
		for (const std::pair<std::string, int> uniform_loc : other.uniform_locations)
			this->add_uniform_location(uniform_loc.first);

		reset_lights();

		assert(id != 0);
	}

	shader::~shader()
	{
		assert(id != 0 && "Tried to destroy a shader that wasn't loaded");
		assert(birb::opengl_initialized);

		birb::log("Shader destroyed [" + vertex_shader_name + ", " + fragment_shader_name + "] (" + birb::ptr_to_str(this) + ")");
		glDeleteProgram(this->id);
	}

	void shader::activate()
	{
		glUseProgram(this->id);
	}

	void shader::reset_lights()
	{
		PROFILER_SCOPE_RENDER_FN()

		// Point lights
		for (u16 i = 0; i < point_light_count; ++i)
		{
			// Set the lights to black
			set(shader_uniforms::point_lights::ambient,  vec3<float>( 0.0f, 0.0f, 0.0f ), i);
			set(shader_uniforms::point_lights::diffuse,  vec3<float>( 0.0f, 0.0f, 0.0f ), i);
			set(shader_uniforms::point_lights::specular, vec3<float>( 0.0f, 0.0f, 0.0f ), i);

			// Set the position to zero
			set(shader_uniforms::point_lights::position, vec3<float>(0.0f, 0.0f, 0.0f), i);

			// Keep the attenuation values
			set(shader_uniforms::point_lights::constant, 1.0f, i);
			set(shader_uniforms::point_lights::linear, 0.09f, i);
			set(shader_uniforms::point_lights::quadratic, 0.032f, i);
		}

		update_directional_light();
	}

	void shader::add_default_3d_matrix_uniforms()
	{
		add_uniform_location("model");
		add_uniform_location("view");
		add_uniform_location("projection");
	}

	void shader::update_directional_light()
	{
		PROFILER_SCOPE_RENDER_FN()

		set(shader_uniforms::directional_light::direction, directional_direction);
		set(shader_uniforms::directional_light::ambient, directional_ambient);
		set(shader_uniforms::directional_light::diffuse, directional_diffuse);
		set(shader_uniforms::directional_light::specular, directional_specular);
	}

	void shader::update_point_lights()
	{
		PROFILER_SCOPE_RENDER_FN()

		// Point lights
		for (u16 i = 0; i < point_light_count; ++i)
		{
			set(shader_uniforms::point_lights::ambient,  point_lights.at(i).ambient, i);
			set(shader_uniforms::point_lights::diffuse,  point_lights.at(i).diffuse, i);
			set(shader_uniforms::point_lights::specular, point_lights.at(i).specular, i);

			set(shader_uniforms::point_lights::position, point_lights.at(i).position, i);

			set(shader_uniforms::point_lights::constant, 	point_lights.at(i).attenuation_constant, i);
			set(shader_uniforms::point_lights::linear,		point_lights.at(i).attenuation_linear, i);
			set(shader_uniforms::point_lights::quadratic,	point_lights.at(i).attenuation_quadratic, i);
		}
	}

	bool shader::has_uniform_var(const std::string& name) const
	{
		return uniform_locations.contains(name);
	}

	void shader::set_int(const std::string& name, const i32 value)
	{
		assert(!name.empty());

		add_uniform_location(name);

		activate();
		glUniform1i(uniform_locations.at(name), value);
	}

	void shader::set_diffuse_color(const color& color)
	{
		set(shader_uniforms::material_color::diffuse, color);
	}

	void shader::set_specular_color(const color& color)
	{
		set(shader_uniforms::material_color::specular, color);
	}

	void shader::set_shininess(const float shininess)
	{
		set(shader_uniforms::material_color::shininess, shininess);
	}

	void shader::apply_color_material(const component::material& material)
	{
		set_diffuse_color(material.diffuse);
		set_specular_color(material.specular);
		set_shininess(material.shininess);
	}

	void shader::draw_editor_ui()
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(!vertex_shader_name.empty());
		assert(!fragment_shader_name.empty());

		ImGui::Text("Vertex: %s", vertex_shader_name.c_str());
		ImGui::Text("Fragment: %s", fragment_shader_name.c_str());
		ImGui::Text("Address: %s", birb::ptr_to_str(this).c_str());
		ImGui::Spacing();

		if (ImGui::TreeNode("Uniforms"))
		{
			auto it = uniform_locations.begin();
			while (it != uniform_locations.end())
			{
				ImGui::BulletText("%s", (*it).first.c_str());
				it++;
			}

			ImGui::TreePop();
		}
	}

	std::string shader::collapsing_header_name() const
	{
		return editor_header_name;
	}

	void shader::compile()
	{
		assert(vertex_shader_name != "NULL");
		assert(fragment_shader_name != "NULL");

		compile_shader(vertex_shader_name, fragment_shader_name);
	}

	std::vector<std::string> shader::vertex_shader_name_list()
	{
		return shader_src_vert_names;
	}

	std::vector<std::string> shader::fragment_shader_name_list()
	{
		return shader_src_frag_names;
	}

	void shader::clear_shader_cache()
	{
		PROFILER_SCOPE_MISC_FN()

		birb::log("Clearing shader cache");

		assert(birb::opengl_initialized);

		// Free all of the shaders
		for (const std::pair<std::string, u32> shader : shader_cache)
			glDeleteShader(shader.second);

		shader_cache.clear();
		shader_cache_hit_count = 0;
	}

	size_t shader::shader_cache_size()
	{
		return shader_cache.size();
	}

	size_t shader::shader_cache_hits()
	{
		return shader_cache_hit_count;
	}

	void shader::add_uniform_location(const std::string& name)
	{
		assert(!name.empty() && "Empty uniform name");

		if (try_add_uniform_location(name) == -1)
			birb::log_fatal("Tried to add shader uniform that doesn't exist: " + name + " [" + vertex_shader_name + ", " + fragment_shader_name + "] (" + ptr_to_str(this) + ")");
	}

	i32 shader::try_add_uniform_location(const std::string& name)
	{
		assert(!name.empty() && "Empty uniform name");

		// Don't fetch the uniform location if its already in the hashmap
		i32 location = -1;

		if (!uniform_locations.contains(name))
		{
			activate();
			location = glGetUniformLocation(id, name.c_str());

			if (location != -1)
				uniform_locations[name] = location;
		}
		else
		{
			location = uniform_locations.at(name);
		}

		return location;
	}

	void shader::compile_shader(const std::string& vertex, const std::string& fragment)
	{
		assert(!shader_src.empty() && "The shader source code hashmap is empty");
		assert(!vertex.empty() && "Empty vertex shader name string");
		assert(!fragment.empty() && "Empty fratment shader name string");

		assert(vertex_shader_name == vertex && "Bug in the shader constructor");
		assert(fragment_shader_name == fragment && "Bug in shader the constructor");

		const std::string vertex_name = vertex + "_vert";
		const std::string fragment_name = fragment + "_frag";

		assert(birb::shader_src.contains(vertex_name) && "Tried to use a vertex shader that wasn't in the pregenerated header");
		assert(birb::shader_src.contains(fragment_name) && "Tried to use a fragment shader that wasn't in the pregenerated header");

		const std::string& vertex_src = birb::shader_src.at(vertex_name);
		const std::string& fragment_src = birb::shader_src.at(fragment_name);

		const char* vertex_src_c_str = vertex_src.c_str();
		const char* fragment_src_c_str = fragment_src.c_str();

		// Compile shaders
		{
			PROFILER_SCOPE_RENDER("Shader compiling")

			birb::log("Compiling shader [" + vertex + ", " + fragment + "] (" + birb::ptr_to_str(this) + ")");

			u32 vertex_shader = 0;
			if (!shader_cache.contains(vertex_name))
			{
				vertex_shader = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertex_shader, 1, &vertex_src_c_str, NULL);
				glCompileShader(vertex_shader);
				compile_errors(vertex_shader, "VERTEX");

				shader_cache[vertex_name] = vertex_shader;
				birb::log("Shader program cached: " + vertex_name + " (" + std::to_string(vertex_shader) + ")");
			}
			else
			{
				vertex_shader = shader_cache.at(vertex_name);
				shader_cache_hit_count++;
				birb::log("Loaded shader from cache: " + vertex_name + " (" + std::to_string(vertex_shader) + ")");
			}
			assert(vertex_shader != 0);

			u32 fragment_shader = 0;
			if (!shader_cache.contains(fragment_name))
			{
				fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragment_shader, 1, &fragment_src_c_str, NULL);
				glCompileShader(fragment_shader);
				compile_errors(fragment_shader, "FRAGMENT");

				shader_cache[fragment_name] = fragment_shader;
				shader_cache_hit_count++;
				birb::log("Shader program cached: " + fragment_name + " (" + std::to_string(fragment_shader) + ")");
			}
			else
			{
				fragment_shader = shader_cache.at(fragment_name);
				birb::log("Loaded shader from cache: " + fragment_name + " (" + std::to_string(fragment_shader) + ")");
			}
			assert(fragment_shader != 0);

			this->id = glCreateProgram();
			glAttachShader(this->id, vertex_shader);
			glAttachShader(this->id, fragment_shader);
			glLinkProgram(this->id);
			compile_errors(id, "PROGRAM");
		}
	}

	void shader::compile_errors(u32 shader, const std::string& type)
	{
		constexpr i32 LOG_BUFFER_SIZE = 1024;

		i32 has_compiled = true;
		char info_log[LOG_BUFFER_SIZE];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_error("Shader failed to compile: " + type + " | " + info_log);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetProgramInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_error("Shader failed to link: " + type + " | " + info_log);
			}
		}
	}
}
