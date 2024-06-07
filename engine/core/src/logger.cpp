#include "Globals.hpp"
#include "Logger.hpp"

#include <glad/gl.h>
#include <stb_sprintf.h>

namespace birb
{
	std::string ptr_to_str(const void* ptr)
	{
		constexpr u8 buf_size = 18;
		char buf[buf_size];
		stbsp_snprintf(buf, buf_size, "%p", ptr);
		return "0x" + std::string(buf);
	}

	void process_gl_errors(const u32 stacktrace_scope_skip_amount)
	{
		// Don't attempt to look for OpenGL errors if OpenGL has not been
		// initialized yet or is not initialized anymore
		if (!g_opengl_initialized)
			return;

#ifndef NDEBUG
		static constexpr char opengl_error_text[] = "OpenGL error: ";

		GLenum err;

		while ((err = glGetError()) != GL_NO_ERROR)
		{
			switch (err)
			{
				case GL_INVALID_ENUM:
					log_error_no_trace(opengl_error_text, "GL_INVALID_ENUM");
					break;

				case GL_INVALID_VALUE:
					log_error_no_trace(opengl_error_text, "GL_INVALID_VALUE");
					break;

				case GL_INVALID_OPERATION:
					log_error_no_trace(opengl_error_text, "GL_INVALID_OPERATION");
					break;

				case 0x0503:
					log_error_no_trace(opengl_error_text, "GL_STACK_OVERFLOW");
					break;

				case 0x0504:
					log_error_no_trace(opengl_error_text, "GL_STACK_UNDERFLOW");
					break;

				case GL_OUT_OF_MEMORY:
					log_error_no_trace(opengl_error_text, "GL_OUT_OF_MEMORY");
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					log_error_no_trace(opengl_error_text, "GL_INVALID_FRAMEBUFFER_OPERATION");
					break;

				default:
					log_error_no_trace("Unknown OpenGL error: ", err);
					break;
			}

			cpptrace::generate_trace(1 + stacktrace_scope_skip_amount).print_with_snippets();
		}
#endif
	}
}
