#pragma once

#include "VBO.hpp"

namespace birb
{
	// Vertex array object
	class vao
	{
	public:
		vao();
		vao(const vao&) = delete;
		~vao();

		// Reference to the vertex array object
		u32 id = 0;

		void link_vbo(birb::vbo& vbo, u32 layout, u32 num_components, size_t stride, u32 offset);
		void bind();
		void unbind();

#ifndef NDEBUG
		u32 d_total_vbo_vert_count = 0;
#endif

	private:

	};
}
