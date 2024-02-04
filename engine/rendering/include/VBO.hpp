#pragma once

#include <vector>

namespace birb
{
	// Vertex buffer object
	class vbo
	{
	public:
		vbo(std::vector<float> vertices);

		// Reference to the vertex buffer object
		unsigned int id;

		void bind();
		void unbind();
		void unload();

	private:

	};
}
