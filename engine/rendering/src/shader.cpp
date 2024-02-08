#include "IO.hpp"
#include "Shader.hpp"

namespace birb
{
	shader::shader(const std::string& vertex_path, const std::string& fragment_path)
	{
		std::string vertex_src = birb::io::read_file(vertex_path);
		std::string fragment_src = birb::io::read_file(fragment_path);

		const char* vertex_src_c_str = vertex_src.c_str();
		const char* fragment_src_c_str = fragment_src.c_str();

		// Compile shaders
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

	void shader::activate()
	{
		glUseProgram(this->id);
	}

	void shader::unload()
	{
		glDeleteProgram(this->id);
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
}
