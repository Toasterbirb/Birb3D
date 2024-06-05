#include "Assert.hpp"
#include "EBO.hpp"

#include <glad/gl.h>
#include <vector>

namespace birb
{
	ebo::ebo(const std::vector<u32>& indices)
	:buffer(gl_buffer_type::element_array)
	{
		ensure(!indices.empty(), "Empty index array");
		ensure(indices.size() < 33000, "You might wanna check the index count on that model");

		load(indices.data(), indices.size());
	}

	void ebo::load(const u32* indices, const size_t size)
	{
		ensure(size > 0);

		buffer.bind();
		buffer.set_data(sizeof(u32) * size, indices, gl_usage::static_draw);
	}

	u32 ebo::id() const
	{
		return buffer.id();
	}

	void ebo::bind()
	{
		buffer.bind();
	}

	void ebo::unbind()
	{
		buffer.unbind();
	}
}
