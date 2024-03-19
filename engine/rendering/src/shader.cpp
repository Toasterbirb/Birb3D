#include "Shader.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "ShaderSource.hpp"

#include <array>
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace birb
{
	// Cache for compiled shaders
	static std::unordered_map<std::string, unsigned int> shader_cache;
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

		set_diffuse_color(other.diffuse_color);
		set_specular_color(other.specular_color);
		set_shininess(other.shininess);

		assert(id != 0);
	}

	shader::~shader()
	{
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
		for (unsigned int i = 0; i < point_light_count; ++i)
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
		for (unsigned int i = 0; i < point_light_count; ++i)
		{
			set(shader_uniforms::point_lights::ambient,  point_lights[i].ambient, i);
			set(shader_uniforms::point_lights::diffuse,  point_lights[i].diffuse, i);
			set(shader_uniforms::point_lights::specular, point_lights[i].specular, i);

			set(shader_uniforms::point_lights::position, point_lights[i].position, i);

			set(shader_uniforms::point_lights::constant, 	point_lights[i].attenuation_constant, i);
			set(shader_uniforms::point_lights::linear,		point_lights[i].attenuation_linear, i);
			set(shader_uniforms::point_lights::quadratic,	point_lights[i].attenuation_quadratic, i);
		}
	}

	bool shader::has_uniform_var(const std::string& name) const
	{
		return uniform_locations.contains(name);
	}

	void shader::set(const uniform& uniform, int value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::INT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform1i(uniform_locations[uniform.str(index)], value);
	}

	void shader::set(const uniform& uniform, float value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::FLOAT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform1f(uniform_locations[uniform.str(index)], value);
	}

	void shader::set(const uniform& uniform, const glm::vec3 value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::VEC3);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations[uniform.str(index)], value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::vec4 value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::VEC4);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform4f(uniform_locations[uniform.str(index)], value.x, value.y, value.z, value.w);
	}

	void shader::set(const uniform& uniform, const birb::vec3<float> value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::BIRB_VEC3_FLOAT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations[uniform.str(index)], value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::mat4 value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::MAT4);

		add_uniform_location(uniform.str(index));

		activate();
		glUniformMatrix4fv(uniform_locations[uniform.str(index)], 1, GL_FALSE, glm::value_ptr(value));
	}

	void shader::set(const uniform& uniform, const color value, int index)
	{
		assert(!uniform.name.empty());
		assert(uniform.type == uniform_type::BIRB_COLOR);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations[uniform.str(index)], value.r, value.g, value.b);
	}

	void shader::set_int(const std::string& name, const int value)
	{
		assert(!name.empty());

		add_uniform_location(name);

		activate();
		glUniform1i(uniform_locations[name], value);
	}

	void shader::set_diffuse_color(const color& color)
	{
		diffuse_color = color;
		set(shader_uniforms::material_color::diffuse, color);
	}

	void shader::set_specular_color(const color& color)
	{
		specular_color = color;
		set(shader_uniforms::material_color::specular, color);
	}

	void shader::set_shininess(const float shininess)
	{
		this->shininess = shininess;
		set(shader_uniforms::material_color::shininess, shininess);
	}

	void shader::apply_color_material()
	{
		set_diffuse_color(diffuse_color);
		set_specular_color(specular_color);
		set_shininess(shininess);
	}

	void shader::draw_editor_ui()
	{
		PROFILER_SCOPE_RENDER_FN()

		if (ImGui::CollapsingHeader("Shader"))
		{
			assert(!vertex_shader_name.empty());
			assert(!fragment_shader_name.empty());

			ImGui::Text("Vertex: %s", vertex_shader_name.c_str());
			ImGui::Text("Fragment: %s", fragment_shader_name.c_str());
			ImGui::Text("Address: %s", birb::ptr_to_str(this).c_str());
			ImGui::Spacing();

			if (uniform_locations.contains("material.diffuse"))
			{
				if (ImGui::ColorEdit3("Diffuse", *diffuse_color.to_ptr_array().data()))
					set_diffuse_color(diffuse_color);
			}

			if (uniform_locations.contains("material.specular"))
			{
				if (ImGui::ColorEdit3("Specular", *specular_color.to_ptr_array().data()))
					set_specular_color(specular_color);
			}

			if (ImGui::DragFloat("shininess", &shininess, 1.0f, 0.1f, 2048.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				set_shininess(shininess);

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

		// Free all of the shaders
		for (const std::pair<std::string, unsigned int> shader : shader_cache)
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

		// Don't fetch the uniform location if its already in the hashmap
		if (!uniform_locations.contains(name))
		{
			activate();
			int location = glGetUniformLocation(id, name.c_str());

			if (location != -1)
				uniform_locations[name] = location;
			else
				birb::log_warn("Tried to add shader uniform that doesn't exist: " + name);
		}
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

		const std::string& vertex_src = birb::shader_src[vertex_name];
		const std::string& fragment_src = birb::shader_src[fragment_name];

		const char* vertex_src_c_str = vertex_src.c_str();
		const char* fragment_src_c_str = fragment_src.c_str();

		// Compile shaders
		{
			PROFILER_SCOPE_RENDER("Shader compiling")

			birb::log("Compiling shader [" + vertex + ", " + fragment + "] (" + birb::ptr_to_str(this) + ")");

			unsigned int vertex_shader = 0;
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
				vertex_shader = shader_cache[vertex_name];
				shader_cache_hit_count++;
				birb::log("Loaded shader from cache: " + vertex_name + " (" + std::to_string(vertex_shader) + ")");
			}
			assert(vertex_shader != 0);

			unsigned int fragment_shader = 0;
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
				fragment_shader = shader_cache[fragment_name];
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

	void shader::compile_errors(unsigned int shader, const std::string& type)
	{
		constexpr int LOG_BUFFER_SIZE = 1024;

		int has_compiled = true;
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
