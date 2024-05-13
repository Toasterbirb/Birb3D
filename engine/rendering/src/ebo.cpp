#include "Assert.hpp"
#include "EBO.hpp"
#include "Globals.hpp"

#include <glad/gl.h>
#include <vector>

namespace birb
{
	ebo::ebo(const std::vector<u32>& indices)
	{
		ASSERT_MSG(!indices.empty(), "Empty index array");
		ASSERT_MSG(indices.size() < 33000, "You might wanna check the index count on that model");

		load(indices.data(), indices.size());
	}

	ebo::~ebo()
	{
		ASSERT(birb::opengl_initialized);
		glDeleteBuffers(1, &id);
	}

	void ebo::load(const u32* indices, const size_t size)
	{
		ASSERT(id == 0);
		ASSERT(size > 0);

		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(f32) * size, indices, GL_STATIC_DRAW);
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
