#pragma once

#include "VBO.hpp"

namespace birb
{
	// Vertex array object
	class vao
	{
	public:
		vao();

		// Reference to the vertex array object
		unsigned int id;

		void link_vbo(birb::vbo vbo, unsigned int layout);
		void bind();
		void unbind();
		void unload();

	private:

	};
}
