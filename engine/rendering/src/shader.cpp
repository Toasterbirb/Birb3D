#include "Shader.hpp"
#include "Profiling.hpp"
#include "ShaderSource.hpp"

#include <cassert>
#include <glm/gtc/type_ptr.hpp>

namespace birb
{
	shader::shader(const std::string& shader_name)
	{
		compile_shader(shader_name, shader_name);
	}

	shader::shader(const std::string& vertex, const std::string& fragment)
	{
		compile_shader(vertex, fragment);
	}

	void shader::compile_shader(const std::string& vertex, const std::string& fragment)
	{
		assert(!vertex.empty() && "Empty vertex shader name string");
		assert(!fragment.empty() && "Empty fratment shader name string");

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
			unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, &vertex_src_c_str, NULL);
			glCompileShader(vertex_shader);
			compile_errors(vertex_shader, "VERTEX");

			unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, &fragment_src_c_str, NULL);
			glCompileShader(fragment_shader);
			compile_errors(fragment_shader, "FRAGMENT");

			this->id = glCreateProgram();
			glAttachShader(this->id, vertex_shader);
			glAttachShader(this->id, fragment_shader);
			glLinkProgram(this->id);
			compile_errors(id, "PROGRAM");

			// Delete the shaders since they are now in the program
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		}
	}

	shader::~shader()
	{
		glDeleteProgram(this->id);
	}

	void shader::activate()
	{
		glUseProgram(this->id);
	}

	void shader::compile_errors(unsigned int shader, const std::string& type)
	{
		constexpr int LOG_BUFFER_SIZE = 1024;

		int has_compiled;
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

	void shader::add_uniform_location(const std::string& name)
	{
		assert(!name.empty() && "Empty uniform name");
		assert(!uniform_locations.contains(name) && "Duplicate uniform variable");

		activate();
		uniform_locations[name] = glGetUniformLocation(id, name.c_str());
	}

	void shader::add_uniform_location(const std::vector<std::string>& names)
	{
		assert(!names.empty() && "Unnecessary call to add_uniform_location");
		for (const std::string& name : names)
			add_uniform_location(name);
	}

	void shader::set_var_mat4(const std::string& name, glm::mat4 mat4)
	{
		assert(!name.empty() && "Empty uniform name");
		assert(uniform_locations.contains(name) && "Tried to access a uniform variable that wasn't added");

		activate();
		glUniformMatrix4fv(uniform_locations[name], 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void shader::set_var_vec3f(const std::string& name, vec3<float> vector)
	{
		activate();
		assert(uniform_locations.contains(name) && "Tried to access a uniform variable that wasn't added");
		glUniform3f(uniform_locations[name], vector.x, vector.y, vector.z);
	}
}
