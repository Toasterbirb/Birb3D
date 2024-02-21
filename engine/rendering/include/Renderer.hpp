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
		static void toggle_wireframe();

	private:
		unsigned int shader_program;
		unsigned int VAO, VBO;
		static inline bool wireframe_mode = false;
	};
}
