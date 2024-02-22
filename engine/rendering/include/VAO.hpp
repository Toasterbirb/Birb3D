#pragma once

#include "VBO.hpp"

#include <glad/gl.h>

namespace birb
{
	// Vertex array object
	class vao
	{
	public:
		vao();
		~vao();

		// Reference to the vertex array object
		unsigned int id;

		void link_vbo(birb::vbo& vbo, unsigned int layout, unsigned int num_components, GLenum type, size_t stride, void* offset);
		void bind();
		void unbind();

	private:

	};
}
