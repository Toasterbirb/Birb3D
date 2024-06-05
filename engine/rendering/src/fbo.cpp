#include "Assert.hpp"
#include "FBO.hpp"
#include "GLSupervisor.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "RBO.hpp"

#include <glad/gl.h>

namespace birb
{
	fbo::fbo(const vec2<i32>& dimensions, const color_format format, const u8 frame_buffer_texture_slot)
	{
		GL_SUPERVISOR_SCOPE();
		ensure(dimensions.x > 0);
		ensure(dimensions.y > 0);
		ensure(g_opengl_initialized);

		texture_slot = frame_buffer_texture_slot;

		glGenFramebuffers(1, &id);

		bind();
		reload_frame_buffer_texture(dimensions, format);

		// Test the framebuffer
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			birb::log_error("Couldn't create a new framebuffer");
		unbind();

		// Make sure that the dimensions are correct
		ensure(frame_buffer.size().x == dimensions.x);
		ensure(frame_buffer.size().y == dimensions.y);
		ensure(d_currently_bound_fbo == 0);
	}

	fbo::~fbo()
	{
		GL_SUPERVISOR_SCOPE();
		ensure(id != 0);
		ensure(birb::g_opengl_initialized);
		glDeleteBuffers(1, &id);

		render_buffer_object.reset();
	}

	void fbo::bind()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, id);

#ifndef NDEBUG
		d_currently_bound_fbo = id;
#endif
	}

	void fbo::unbind()
	{
		PROFILER_SCOPE_RENDER_FN();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifndef NDEBUG
		d_currently_bound_fbo = 0;
#endif
	}

	void fbo::bind_frame_buffer()
	{
		GL_SUPERVISOR_SCOPE();
		ensure(frame_buffer.id != 0);
		ensure(frame_buffer.size().x > 0);
		ensure(frame_buffer.size().y > 0);
		frame_buffer.bind();
	}

	void fbo::unbind_frame_buffer()
	{
		frame_buffer.unbind();
	}

	vec2<i32> fbo::frame_buffer_dimensions() const
	{
		return frame_buffer.size();
	}

	u32 fbo::frame_buffer_id() const
	{
		return frame_buffer.id;
	}

	texture* fbo::frame_buffer_ptr()
	{
		return &frame_buffer;
	}

	void fbo::clear()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(d_currently_bound_fbo == id, "Remember to bind the FBO before modifying it");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void fbo::reload_frame_buffer_texture(const vec2<i32>& dimensions, const color_format format)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(birb::g_opengl_initialized);
		ensure(d_currently_bound_fbo == id, "Remember to bind the FBO before modifying it");

		// Texture
		if (frame_buffer.id != 0)
		{
			glDeleteTextures(1, &frame_buffer.id);
			frame_buffer.id = 0;
		}

		frame_buffer.create_empty(dimensions, format, texture_slot);
		attach_texture(frame_buffer, format);

		// Render buffer object
		render_buffer_object.reset();

		setup_rbo(dimensions);
	}

	void fbo::attach_texture(const texture& texture, const color_format format)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(id != 0);
		ensure(texture.id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, this->id);

		// Treat depth maps differently
		if (format == color_format::DEPTH)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
		}
	}

	void fbo::setup_rbo(vec2<i32> dimensions)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(!render_buffer_object, "The render buffer object needs to be destroyed before creating a new one");

		render_buffer_object = std::make_unique<rbo>(dimensions);
	}
}
