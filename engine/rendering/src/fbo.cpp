#include "FBO.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"

#include <cassert>
#include <glad/gl.h>

namespace birb
{
	fbo::fbo(const vec2<int>& dimensions)
	{
		glGenFramebuffers(1, &id);
		reload_frame_buffer_texture(dimensions);

		// Test the framebuffer
		bind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			birb::log_error("Couldn't create a new framebuffer");
		unbind();
	}

	fbo::~fbo()
	{
		assert(id != 0);
		assert(birb::opengl_initialized);
		glDeleteBuffers(1, &id);

		render_buffer_object.reset();
	}

	void fbo::bind()
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(id != 0);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void fbo::unbind()
	{
		PROFILER_SCOPE_RENDER_FN()

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void fbo::reload_frame_buffer_texture(const vec2<int>& dimensions)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(birb::opengl_initialized);

		// Texture
		if (frame_buffer.id != 0)
		{
			glDeleteTextures(1, &frame_buffer.id);
			frame_buffer.id = 0;
		}

		frame_buffer.create_empty(dimensions);
		attach_texture(frame_buffer);

		// Render buffer object
		render_buffer_object.reset();

		setup_rbo(dimensions);
	}

	texture& fbo::frame_buffer_texture()
	{
		return this->frame_buffer;
	}

	void fbo::attach_texture(const texture& texture)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(id != 0);
		assert(texture.id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
	}

	void fbo::setup_rbo(vec2<int> dimensions)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(!render_buffer_object && "The render buffer object needs to be destroyed before creating a new one");

		bind();
		render_buffer_object = std::make_unique<rbo>(dimensions);
	}
}
