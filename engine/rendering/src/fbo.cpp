#include "Assert.hpp"
#include "FBO.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "RBO.hpp"

#include <glad/gl.h>

namespace birb
{
	fbo::fbo(const vec2<i32>& dimensions)
	{
		ensure(dimensions.x > 0);
		ensure(dimensions.y > 0);

		glGenFramebuffers(1, &id);
		reload_frame_buffer_texture(dimensions);

		// Act as if the framebuffer was not bound at this point
		// This will skip the asset in the bind() function
		is_bound = false;

		// Test the framebuffer
		bind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			birb::log_error("Couldn't create a new framebuffer");
		unbind();
	}

	fbo::~fbo()
	{
		ensure(id != 0);
		ensure(birb::opengl_initialized);
		glDeleteBuffers(1, &id);

		render_buffer_object.reset();
	}

	void fbo::bind()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(!is_bound, "This FBO was already bound before. Binding it again is unnecessary");
		ensure(id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, id);
		is_bound = true;
	}

	void fbo::unbind()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(is_bound, "FBO was not bound, so it cannot be unbound");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		is_bound = false;
	}

	void fbo::bind_frame_buffer()
	{
		frame_buffer.bind();
	}

	void fbo::unbind_frame_buffer()
	{
		frame_buffer.unbind();
	}

	u32 fbo::frame_buffer_id() const
	{
		return frame_buffer.id;
	}

	void fbo::clear()
	{
		PROFILER_SCOPE_RENDER_FN();

		const bool was_bound = is_bound;

		if (!was_bound)
			bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!was_bound)
			unbind();
	}

	void fbo::reload_frame_buffer_texture(const vec2<i32>& dimensions)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(birb::opengl_initialized);

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

	void fbo::attach_texture(const texture& texture)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(id != 0);
		ensure(texture.id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
	}

	void fbo::setup_rbo(vec2<i32> dimensions)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(!render_buffer_object, "The render buffer object needs to be destroyed before creating a new one");

		const bool was_bound = is_bound;

		if (!was_bound)
			bind();

		render_buffer_object = std::make_unique<rbo>(dimensions);

		if (!was_bound)
			unbind();
	}
}
