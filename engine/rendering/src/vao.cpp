#include "VAO.hpp"

#include <glad/gl.h>

namespace birb
{
	vao::vao()
	{
		glGenVertexArrays(1, &id);
	}

	vao::~vao()
	{
		glDeleteVertexArrays(1, &id);
	}

	void vao::link_vbo(birb::vbo& vbo, unsigned int layout, unsigned int num_components, size_t stride, unsigned int offset)
	{
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
