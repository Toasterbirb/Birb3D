#include "VAO.hpp"

namespace birb
{
	vao::vao()
	{
		glGenVertexArrays(1, &id);
	}

	void vao::link_vbo(birb::vbo vbo, unsigned int layout)
	{
		vbo.bind();

		glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
