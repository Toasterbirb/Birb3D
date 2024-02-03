#pragma once

#include <vector>

namespace birb
{
	class renderer
	{
	public:
		renderer();
		~renderer();

		void draw_verts(std::vector<float> verts);

	private:
		unsigned int shader_program;
		unsigned int VAO, VBO;
	};
}
