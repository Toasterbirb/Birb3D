#include "RBO.hpp"
#include "Profiling.hpp"

#include <GL/gl.h>

namespace birb
{
	rbo::rbo(const vec2<int>& dimensions)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(id == 0);
		glGenRenderbuffers(1, &id);
		assert(id != 0 && "Failed to create a render buffer object");

		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
	}

	rbo::~rbo()
	{
		assert(id != 0);
		glDeleteRenderbuffers(1, &id);
	}
}
