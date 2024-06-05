#include "Assert.hpp"
#include "UBO.hpp"

#include <glad/gl.h>

namespace birb
{
	ubo::ubo(const uniform_block& block)
	:size(block.size)
	{
		create_ubo(block.bind_point, block.size, block.static_draw);
	}

	ubo::ubo(const u32 bind_point, const u32 size_in_bytes, const bool static_draw)
	:size(size_in_bytes)
	{
		create_ubo(bind_point, size_in_bytes, static_draw);
	}

	ubo::~ubo()
	{
		if (id != 0)
			glDeleteBuffers(1, &id);
	}

	ubo::ubo(ubo&& other)
	{
		id = other.id;
		size = other.size;

		other.id = 0;
		other.size = 0;
	}

	void ubo::bind() const
	{
		ensure(id != 0);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
	}

	void ubo::unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void ubo::update_data(const void* data, const std::intptr_t size, const std::intptr_t offset) const
	{
		ensure(id != 0);
		ensure(data != nullptr);
		ensure(size <= this->size);
		ensure(offset <= this->size);

		bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		unbind();
	}

	void ubo::create_ubo(const u32 bind_point, const u32 size_in_bytes, const bool static_draw)
	{
		ensure(size_in_bytes > 0);

		// Create and allocate the uniform buffer
		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);

		if (static_draw)
			glBufferData(GL_UNIFORM_BUFFER, size_in_bytes, NULL, GL_STATIC_DRAW);
		else
			glBufferData(GL_UNIFORM_BUFFER, size_in_bytes, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, bind_point, id, 0, size_in_bytes);

		ensure(id != 0);
	}
}
