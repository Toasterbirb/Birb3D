#include "Assert.hpp"
#include "UBO.hpp"

#include <glad/gl.h>

namespace birb
{
	ubo::ubo(const uniform_block& block)
	:buffer(gl_buffer_type::uniform), size(block.size)
	{
		create_ubo(block.bind_point, block.size, block.usage);
	}

	ubo::ubo(const u32 bind_point, const u32 size_in_bytes, const gl_usage usage)
	:buffer(gl_buffer_type::uniform), size(size_in_bytes)
	{
		create_ubo(bind_point, size_in_bytes, usage);
	}

	void ubo::bind() const
	{
		buffer.bind();
	}

	void ubo::unbind()
	{
		gl_buffer::unbind(gl_buffer_type::uniform);
	}

	void ubo::update_data(const void* data, const std::intptr_t size, const std::intptr_t offset) const
	{
		ensure(data != nullptr);
		ensure(size <= this->size);
		ensure(offset <= this->size);

		bind();
		buffer.update_data(size, data, offset);
		unbind();
	}

	void ubo::create_ubo(const u32 bind_point, const u32 size_in_bytes, const gl_usage usage)
	{
		ensure(size_in_bytes > 0);

		// Create and allocate the uniform buffer
		buffer.bind();
		buffer.set_data(size_in_bytes, NULL, usage);
		buffer.unbind();

		glBindBufferRange(GL_UNIFORM_BUFFER, bind_point, buffer, 0, size_in_bytes);
	}
}
