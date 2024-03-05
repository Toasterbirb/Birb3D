#include "FBO.hpp"
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
		glDeleteBuffers(1, &id);

		assert(rbo != 0);
		glDeleteRenderbuffers(1, &rbo);
	}

	void fbo::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void fbo::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void fbo::reload_frame_buffer_texture(const vec2<int>& dimensions)
	{
		PROFILER_SCOPE_RENDER_FN()

		// Texture
		if (frame_buffer.id != 0)
		{
			glDeleteTextures(1, &frame_buffer.id);
			frame_buffer.id = 0;
		}

		frame_buffer.create_empty(dimensions);
		attach_texture(frame_buffer);

		// Render buffer object
		if (rbo != 0)
		{
			glDeleteRenderbuffers(1, &rbo);
			rbo = 0;
		}

		setup_rbo(dimensions);
	}

	texture& fbo::frame_buffer_texture()
	{
		return this->frame_buffer;
	}

	void fbo::attach_texture(const texture& texture)
	{
		assert(id != 0);
		assert(texture.id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
	}

	void fbo::setup_rbo(vec2<int> dimensions)
	{
		PROFILER_SCOPE_RENDER_FN()

		bind();
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}
}
