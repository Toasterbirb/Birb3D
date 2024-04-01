#include "EBO.hpp"
#include "Globals.hpp"

#include <cassert>
#include <vector>

namespace birb
{
	ebo::ebo(const std::vector<u32>& indices)
	{
		assert(!indices.empty() && "Empty index array");
		assert(indices.size() < 33000 && "You might wanna check the index count on that model");

		load(indices.data(), indices.size());
	}

	ebo::~ebo()
	{
		assert(birb::opengl_initialized);
		glDeleteBuffers(1, &id);
	}

	void ebo::load(const u32* indices, const size_t size)
	{
		assert(id == 0);
		assert(size > 0);

		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size, indices, GL_STATIC_DRAW);
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
