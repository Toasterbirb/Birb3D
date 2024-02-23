#include "Shader.hpp"
#include "Profiling.hpp"
#include "ShaderSource.hpp"

#include <cassert>

namespace birb
{
	shader::shader(const std::string& vertex, const std::string& fragment)
	{
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
				birb::log_error("Shader failed to compiled: " + type);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetProgramInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_error("Shader failed to link: " + type);
			}
		}
	}

	void shader::add_uniform_location(const std::string& name)
	{
		activate();
		uniform_locations[name] = glGetUniformLocation(id, name.c_str());
	}
}
