#include "VBO.hpp"

#include <vector>

namespace birb
{
	vbo::vbo(std::vector<float> vertices)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	void vbo::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void vbo::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void vbo::unload()
	{
		glDeleteBuffers(1, &id);
	}
}
