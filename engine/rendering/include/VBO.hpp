#pragma once

#include <vector>

namespace birb
{
	// Vertex buffer object
	class vbo
	{
	public:
		explicit vbo(std::vector<float> vertices);
		~vbo();

		// Reference to the vertex buffer object
		unsigned int id;

		void bind();
		void unbind();

	private:

	};
}
