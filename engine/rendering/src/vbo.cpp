#include "Assert.hpp"
#include "Globals.hpp"
#include "VBO.hpp"

#include <glad/gl.h>
#include <vector>

namespace birb
{
	vbo::vbo()
	:buffer(gl_buffer_type::array)
	{}

	vbo::vbo(const std::vector<f32>& vertices, const bool static_draw)
	:buffer(gl_buffer_type::array)
	{
		ensure(!vertices.empty(), "Empty data array");
		bind();
		set_data(vertices.data(), vertices.size(), static_draw);
		unbind();
	}

	u32 vbo::id() const
	{
		return buffer.id();
	}

	void vbo::set_data(const std::vector<f32>& data, const bool static_draw) const
	{
		ensure(!data.empty());
		set_data(data.data(), data.size(), static_draw);
	}

	void vbo::set_data(const f32* data, const size_t size, const bool static_draw) const
	{
		ensure(size > 0);
		ensure(d_currently_bound_vbo == buffer.id(), "Remember to bind the VBO before modifying it");

		if (static_draw)
			buffer.set_data(sizeof(f32) * size, data, gl_usage::static_draw);
		else
			buffer.set_data(sizeof(f32) * size, data, gl_usage::dynamic_draw);
	}

	void vbo::update_data(const f32* data, const size_t size, const u32 offset) const
	{
		ensure(size > 0);
		ensure(d_currently_bound_vbo == buffer.id(), "Remember to bind the VBO before modifying it");

		buffer.update_data(size, data, offset);
	}

	void vbo::enable_vertex_attrib_array(const u32 index) const
	{
		ensure(d_currently_bound_vbo == buffer.id(), "Remember to bind the VBO before modifying it");
		glEnableVertexAttribArray(index);
	}

	void vbo::disable_vertex_attrib_array(const u32 index) const
	{
		ensure(d_currently_bound_vbo == buffer.id(), "Remember to bind the VBO before modifying it");
		glDisableVertexAttribArray(index);
	}

	void vbo::set_vertex_attrib_ptr(const u32 layout_index,
				const u32 component_index,
				const size_t component_size,
				const u32 components_per_vert) const
	{
		ensure(d_currently_bound_vbo == buffer.id(), "Remember to bind the VBO before modifying it");
		glVertexAttribPointer(layout_index + component_index,
				components_per_vert,
				GL_FLOAT, GL_FALSE,
				components_per_vert * component_size,
				reinterpret_cast<void*>(component_index * component_size));
	}

	void vbo::bind() const
	{
		buffer.bind();

#ifndef NDEBUG
		d_currently_bound_vbo = buffer.id();
#endif
	}

	void vbo::unbind()
	{
		gl_buffer::unbind(gl_buffer_type::array);

#ifndef NDEBUG
		d_currently_bound_vbo = 0;
#endif
	}
}
