#pragma once

#include <vector>

namespace birb
{
	// Vertex buffer object
	class vbo
	{
	public:
		explicit vbo(const std::vector<float>& vertices);
		vbo(const vbo&) = delete;
		~vbo();

		// Reference to the vertex buffer object
		unsigned int id;

		void bind();
		void unbind();

	private:

	};
}
