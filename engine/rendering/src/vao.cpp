#include "Globals.hpp"
#include "Profiling.hpp"
#include "VAO.hpp"

#include <cassert>
#include <glad/gl.h>

namespace birb
{
	vao::vao()
	{
		glGenVertexArrays(1, &id);
	}

	vao::~vao()
	{
		assert(birb::opengl_initialized);
		glDeleteVertexArrays(1, &id);
	}

	void vao::link_vbo(birb::vbo& vbo, u32 layout, u32 num_components, size_t stride, u32 offset)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(vbo.id != 0);
		assert(num_components != 0 && "Invalid amount of components");
		assert(stride != 0 && "Invalid stride");

		vbo.bind();

		glVertexAttribPointer(layout, num_components, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
		glEnableVertexAttribArray(layout);

		vbo.unbind();
	}

	void vao::bind()
	{
		glBindVertexArray(id);
	}

	void vao::unbind()
	{
		glBindVertexArray(0);
	}
}
