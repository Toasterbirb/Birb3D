#include "Assert.hpp"
#include "Color.hpp"
#include "Globals.hpp"
#include "IO.hpp"
#include "Logger.hpp"
#include "Material.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "ShaderSource.hpp"
#include "ShaderUniforms.hpp"

#include <array>
#include <filesystem>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace birb
{
	// Cache for compiled shaders
	static std::unordered_map<std::string, u32> shader_cache;
	static size_t shader_cache_hit_count = 0;

	static const std::string missing_shader_vert = "default_vert";
	static const std::string missing_shader_frag = "missing_shader_frag";

	shader::shader()
	{}

	shader::shader(const std::string& shader_name)
	:vertex_shader_name(shader_name), fragment_shader_name(shader_name)
	{
		static_assert(shader_type::vertex == GL_VERTEX_SHADER);
		static_assert(shader_type::fragment == GL_FRAGMENT_SHADER);

		static_assert(point_light_count > 0);
		static_assert(point_light_count < 8192, "Arbitrary limitation, but you are probably doing something wrong");

		compile_shader(shader_name, shader_name);
		ensure(id != 0);
	}

	shader::shader(const std::string& vertex, const std::string& fragment)
	:vertex_shader_name(vertex), fragment_shader_name(fragment)
	{
		compile_shader(vertex, fragment);
		ensure(id != 0);
	}

	shader::shader(const shader& other)
	{
		PROFILER_SCOPE_MISC("Shader copy");

		birb::log("Shader copy causing a recompile (" + birb::ptr_to_str(&other) + " -> " + birb::ptr_to_str(this) + ")");

		id = 0;
		vertex_shader_name		= other.vertex_shader_name;
		fragment_shader_name	= other.fragment_shader_name;

		compile_shader(other.vertex_shader_name, other.fragment_shader_name);

		reset_lights();

		ensure(id != 0);
	}

	shader::~shader()
	{
		ensure(id != 0, "Tried to destroy a shader that wasn't loaded");
		ensure(birb::g_opengl_initialized);

		birb::log("Shader destroyed [" + vertex_shader_name + ", " + fragment_shader_name + "] (" + birb::ptr_to_str(this) + ")");
		glDeleteProgram(this->id);
	}

	void shader::activate()
	{
		glUseProgram(this->id);

#ifndef NDEBUG
		d_currently_active_shader = id;
#endif
	}

	void shader::reset_lights()
	{
		PROFILER_SCOPE_RENDER_FN();

		// Point lights
		for (u16 i = 0; i < point_light_count; ++i)
		{
			// Set the lights to black
			set(shader_uniforms::point_lights::ambient,  vec3<f32>( 0.0f, 0.0f, 0.0f ), i);
			set(shader_uniforms::point_lights::diffuse,  vec3<f32>( 0.0f, 0.0f, 0.0f ), i);
			set(shader_uniforms::point_lights::specular, vec3<f32>( 0.0f, 0.0f, 0.0f ), i);

			// Set the position to zero
			set(shader_uniforms::point_lights::position, vec3<f32>(0.0f, 0.0f, 0.0f), i);

			// Keep the attenuation values
			set(shader_uniforms::point_lights::constant, 1.0f, i);
			set(shader_uniforms::point_lights::linear, 0.09f, i);
			set(shader_uniforms::point_lights::quadratic, 0.032f, i);
		}

		update_directional_light();
	}

	void shader::update_directional_light()
	{
		PROFILER_SCOPE_RENDER_FN();

		set(shader_uniforms::directional_light::direction, directional_direction);
		set(shader_uniforms::directional_light::ambient, directional_ambient);
		set(shader_uniforms::directional_light::diffuse, directional_diffuse);
		set(shader_uniforms::directional_light::specular, directional_specular);
	}

	void shader::update_point_lights()
	{
		PROFILER_SCOPE_RENDER_FN();

		set(shader_uniforms::point_lights::count, point_light_count);

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
		return glGetUniformLocation(id, name.c_str()) != -1;
	}

	i32 shader::uniform_location(const std::string& name) const
	{
		const i32 location = glGetUniformLocation(id, name.c_str());
		ensure(location != -1, "Nonexistent uniform");
		return location;
	}

	void shader::set_int(const std::string& name, const i32 value)
	{
		ensure(!name.empty());

		activate();
		glUniform1i(uniform_location(name), value);
	}

	void shader::set_diffuse_color(const color& color)
	{
		set(shader_uniforms::material_color::diffuse, color);
	}

	void shader::set_specular_color(const color& color)
	{
		set(shader_uniforms::material_color::specular, color);
	}

	void shader::set_shininess(const f32 shininess)
	{
		set(shader_uniforms::material_color::shininess, shininess);
	}

	void shader::apply_color_material(const material& material)
	{
		set_diffuse_color(material.diffuse);
		set_specular_color(material.specular);
		set_shininess(material.shininess);
		set(shader_uniforms::material::type, material.type);
	}

	void shader::draw_editor_ui()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(!vertex_shader_name.empty());
		ensure(!fragment_shader_name.empty());

		ImGui::Text("Vertex: %s", vertex_shader_name.c_str());
		ImGui::Text("Fragment: %s", fragment_shader_name.c_str());
		ImGui::Text("Address: %s", birb::ptr_to_str(this).c_str());
	}

	std::string shader::collapsing_header_name() const
	{
		return editor_header_name;
	}

	void shader::compile()
	{
		ensure(vertex_shader_name != "NULL");
		ensure(fragment_shader_name != "NULL");

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

	bool shader::is_missing() const
	{
		return _is_missing;
	}

	void shader::clear_shader_cache()
	{
		PROFILER_SCOPE_MISC_FN();

		birb::log("Clearing shader cache");

		ensure(birb::g_opengl_initialized);

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

	std::string shader::load_shader_src(const std::string& shader_name) const
	{
		ensure(!shader_name.empty());

		// Try to fetch the shader from builtin shaders
		if (shader_src.contains(shader_name))
			return shader_src.at(shader_name);

		// Resort to loading external shaders
		ensure(!shader_src_search_paths.empty(), "Tried to find shader source code with an empty search path array");

		const std::string file_name = shader_name + ".glsl";

		for (const std::string& path : shader_src_search_paths)
		{
			const std::string file_path = path + '/' + file_name;
			if (std::filesystem::exists(file_path))
				return io::read_file(file_path);
		}

		birb::log_error("External shader file [" + shader_name + "] could not be found");

		return "";
	}

	void shader::compile_shader(const std::string& vertex, const std::string& fragment)
	{
		ensure(!shader_src.empty(), "The shader source code hashmap is empty");
		ensure(!vertex.empty(), "Empty vertex shader name string");
		ensure(!fragment.empty(), "Empty fratment shader name string");

		ensure(vertex_shader_name == vertex, "Bug in the shader constructor");
		ensure(fragment_shader_name == fragment, "Bug in shader the constructor");

		std::string vertex_name = vertex + "_vert";
		std::string fragment_name = fragment + "_frag";

		std::string vertex_src, fragment_src;

		// Fetch source code for the shaders
		vertex_src = load_shader_src(vertex_name);
		fragment_src = load_shader_src(fragment_name);

		// In case no shaders got loaded for either vertex or fragment shaders,
		// use the "missing shader" -shader
		if (vertex_src.empty() || fragment_src.empty())
		{
			ensure(shader_src.contains(missing_shader_vert));
			ensure(shader_src.contains(missing_shader_frag));

			vertex_shader_name = "missing";
			fragment_shader_name = "missing";
			_is_missing = true;

			vertex_name = missing_shader_vert;
			fragment_name = missing_shader_frag;

			vertex_src = shader_src.at(missing_shader_vert);
			fragment_src = shader_src.at(missing_shader_frag);
		}

		ensure(!vertex_src.empty());
		ensure(!fragment_src.empty());

		const char* vertex_src_c_str = vertex_src.c_str();
		const char* fragment_src_c_str = fragment_src.c_str();

		// Compile shaders
		{
			PROFILER_SCOPE_RENDER("Shader compiling");

			birb::log("Compiling shader [", vertex, ", ", fragment, "] (", birb::ptr_to_str(this), ")");

			u32 vertex_shader = compile_gl_shader_program(vertex_name, vertex_src_c_str, shader_type::vertex);
			ensure(vertex_shader != 0);

			u32 fragment_shader = compile_gl_shader_program(fragment_name, fragment_src_c_str, shader_type::fragment);
			ensure(fragment_shader != 0);

			this->id = glCreateProgram();
			glAttachShader(this->id, vertex_shader);
			glAttachShader(this->id, fragment_shader);
			glLinkProgram(this->id);
			compile_errors(id, shader_type::program);
		}

		// Bind the shader to different uniform blocks

		const i32 uniform_block_index_view_matrices = glGetUniformBlockIndex(this->id, shader_uniforms::block::view_matrices.block_name);
		ensure(uniform_block_index_view_matrices != -1, "The view matrix uniform block could not be found in a shader");
		glUniformBlockBinding(id, uniform_block_index_view_matrices, shader_uniforms::block::view_matrices.bind_point);

		const i32 uniform_block_index_projection_matrices = glGetUniformBlockIndex(this->id, shader_uniforms::block::projection_matrices.block_name);
		ensure(uniform_block_index_projection_matrices != -1, "The projection matrix uniform block could not be found in a shader");
		glUniformBlockBinding(id, uniform_block_index_projection_matrices, shader_uniforms::block::projection_matrices.bind_point);
	}

	u32 shader::compile_gl_shader_program(const std::string& shader_name, const char* shader_src, const shader_type type)
	{
		u32 shader_id = 0;

		if (shader_cache.contains(shader_name))
		{
			shader_id = shader_cache.at(shader_name);
			shader_cache_hit_count++;
			birb::log("Loaded shader from cache: ", shader_name, " (", shader_id, ")");
			return shader_id;
		}

		shader_id = glCreateShader(type);
		glShaderSource(shader_id, 1, &shader_src, NULL);
		glCompileShader(shader_id);
		compile_errors(shader_id, type);

		shader_cache[shader_name] = shader_id;
		birb::log("Shader program cached: ", shader_name, " (", shader_id, ")");
		return shader_id;
	}

	std::string shader::shader_type_to_str(const shader_type type) const
	{
		switch (type)
		{
			case program:
				return "PROGRAM";
				break;

			case vertex:
				return "VERTEX";
				break;

			case fragment:
				return "FRAGMENT";
				break;
		}

		ensure(0, "Unhandled shader type name");
		return "NULL";
	}

	void shader::compile_errors(u32 shader, const shader_type type)
	{
		constexpr i32 LOG_BUFFER_SIZE = 1024;

		i32 has_compiled = true;
		char info_log[LOG_BUFFER_SIZE];

		// Handle errors for shader compiling
		if (type != shader_type::program)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_fatal(3, shader_type_to_str(type) + " shader failed to compile:" + info_log);
			}

			return;
		}

		// Handle errors for shader program linking
		glGetProgramiv(shader, GL_COMPILE_STATUS, &has_compiled);
		if (has_compiled == false)
		{
			glGetProgramInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
			birb::log_fatal(3, "Shader program failed to link:", info_log);
		}
	}
}
