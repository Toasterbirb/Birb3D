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
		unsigned int id = 0;

		void link_vbo(birb::vbo& vbo, unsigned int layout, unsigned int num_components, size_t stride, unsigned int offset);
		void bind();
		void unbind();

	private:

	};
}
