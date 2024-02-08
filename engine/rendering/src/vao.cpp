#include "VAO.hpp"

#include <glad/gl.h>

namespace birb
{
	vao::vao()
	{
		glGenVertexArrays(1, &id);
	}

	void vao::link_vbo(birb::vbo vbo, unsigned int layout, unsigned int num_components, GLenum type, size_t stride, void* offset)
	{
		vbo.bind();

		glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
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

	void vao::unload()
	{
		glDeleteVertexArrays(1, &id);
	}
}
