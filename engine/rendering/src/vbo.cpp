#include "VBO.hpp"

#include <cassert>
#include <vector>

namespace birb
{
	vbo::vbo(const std::vector<float>& vertices)
	{
		assert(!vertices.empty() && "Empty index array");
		assert(vertices.size() < 33000 && "You might wanna check the vert count on that model");

		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	vbo::~vbo()
	{
		glDeleteBuffers(1, &id);
	}

	void vbo::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void vbo::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
