#include "Globals.hpp"
#include "VBO.hpp"

#include <cassert>
#include <vector>

namespace birb
{
	vbo::vbo(const std::vector<f32>& vertices, const bool static_draw)
	{
		assert(!vertices.empty() && "Empty vertex array");
		assert(vertices.size() < 33000 && "You might wanna check the vert count on that model");

		load(vertices.data(), vertices.size(), static_draw);
	}

	vbo::~vbo()
	{
		assert(birb::opengl_initialized);
		glDeleteBuffers(1, &id);
	}

	void vbo::load(const f32* vertices, const size_t size, const bool static_draw)
	{
		assert(id == 0);
		assert(size > 0);

		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);

		if (static_draw)
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * size, vertices, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * size, vertices, GL_DYNAMIC_DRAW);

#ifndef NDEBUG
		d_vert_count = size;
#endif
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
