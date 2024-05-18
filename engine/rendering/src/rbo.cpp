#include "Assert.hpp"
#include "Globals.hpp"
#include "Profiling.hpp"
#include "RBO.hpp"
#include "Vector.hpp"

#include <glad/gl.h>

namespace birb
{
	rbo::rbo(const vec2<i32>& dimensions)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(dimensions.x > 0);
		ensure(dimensions.y > 0);

		ensure(id == 0);
		glGenRenderbuffers(1, &id);
		ensure(id != 0, "Failed to create a render buffer object");

		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
	}

	rbo::~rbo()
	{
		ensure(id != 0);
		ensure(birb::opengl_initialized);
		glDeleteRenderbuffers(1, &id);
	}
}
