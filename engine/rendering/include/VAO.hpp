#pragma once

#include "Types.hpp"

namespace birb
{
	class ebo;
	class vbo;

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
		void link_ebo(birb::ebo& ebo);
		void bind();
		void unbind();

		/**
		 * @return True if an EBO has been linked to this VAO
		 */
		bool contains_ebo() const;

#ifndef NDEBUG
		u32 d_total_vbo_vert_count = 0;
#endif

	private:
		// Has EBO been linked to this VAO
		bool has_ebo = false;
	};
}
