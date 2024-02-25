#include "EBO.hpp"

#include <cassert>
#include <vector>

namespace birb
{
	ebo::ebo(const std::vector<unsigned int>& indices)
	{
		assert(!indices.empty() && "Empty index array");
		assert(indices.size() < 33000 && "You might wanna check the index count on that model");

		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}

	ebo::~ebo()
	{
		glDeleteBuffers(1, &id);
	}

	void ebo::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void ebo::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
