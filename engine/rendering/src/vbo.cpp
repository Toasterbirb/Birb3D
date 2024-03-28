#include "Globals.hpp"
#include "VBO.hpp"

#include <cassert>
#include <vector>

namespace birb
{
	vbo::vbo(const std::vector<float>& vertices)
	{
		assert(!vertices.empty() && "Empty vertex array");
		assert(vertices.size() < 33000 && "You might wanna check the vert count on that model");

		load(vertices.data(), vertices.size());
	}

	vbo::~vbo()
	{
		assert(birb::opengl_initialized);
		glDeleteBuffers(1, &id);
	}

	void vbo::load(const float* vertices, const size_t size)
	{
		assert(id == 0);
		assert(size > 0);

		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);
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
