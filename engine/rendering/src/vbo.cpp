#include "Assert.hpp"
#include "Globals.hpp"
#include "VBO.hpp"

#include <glad/gl.h>
#include <vector>

namespace birb
{
	vbo::vbo()
	{
		allocate_buffer();
	}

	vbo::vbo(const std::vector<f32>& vertices, const bool static_draw)
	{
		ensure(!vertices.empty(), "Empty data array");

		allocate_buffer();
		set_data(vertices.data(), vertices.size(), static_draw);
	}

	vbo::~vbo()
	{
		ensure(birb::opengl_initialized);
		ensure(id != 0);
		glDeleteBuffers(1, &id);
	}

	void vbo::set_data(const std::vector<f32>& data, const bool static_draw) const
	{
		ensure(!data.empty());
		set_data(data.data(), data.size(), static_draw);
	}

	void vbo::set_data(const f32* data, const size_t size, const bool static_draw) const
	{
		ensure(id != 0);
		ensure(size > 0);

		glBindBuffer(GL_ARRAY_BUFFER, id);

		if (static_draw)
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * size, data, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * size, data, GL_DYNAMIC_DRAW);
	}

	void vbo::update_data(const f32* data, const size_t size, const u32 offset) const
	{
		ensure(id != 0);
		ensure(size > 0);

		glBufferSubData(GL_ARRAY_BUFFER, size * offset, size, data);
	}

	void vbo::enable_vertex_attrib_array(const u32 index) const
	{
		ensure(currently_bound_vbo == id, "Remember to bind the VBO before modifying it");
		glEnableVertexAttribArray(index);
	}

	void vbo::disable_vertex_attrib_array(const u32 index) const
	{
		ensure(currently_bound_vbo == id, "Remember to bind the VBO before modifying it");
		glDisableVertexAttribArray(index);
	}

	void vbo::set_vertex_attrib_ptr(const u32 layout_index,
				const u32 component_index,
				const size_t component_size,
				const u32 components_per_vert) const
	{
		ensure(currently_bound_vbo == id, "Remember to bind the VBO before modifying it");
		glVertexAttribPointer(layout_index + component_index,
				components_per_vert,
				GL_FLOAT, GL_FALSE,
				components_per_vert * component_size,
				reinterpret_cast<void*>(component_index * component_size));
	}

	void vbo::allocate_buffer()
	{
		ensure(id == 0);
		glGenBuffers(1, &id);
	}

	void vbo::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		currently_bound_vbo = id;
	}

	void vbo::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		currently_bound_vbo = 0;
	}
}
