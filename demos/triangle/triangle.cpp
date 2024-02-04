#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

int main(void)
{
	birb::window window("Triangle", birb::vec2<int>(800, 800));
	birb::renderer renderer;

	window.init_imgui();

	std::vector<float> verts =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	// Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		// Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	// Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,		// Inner down
	};

	std::vector<unsigned int> indices =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1, // Upper triangle
	};

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	birb::shader shader_program("default_vert.glsl", "default_frag.glsl");

	birb::vao vao1;
	vao1.bind();

	birb::vbo vbo1(verts);
	birb::ebo ebo1(indices);

	vao1.link_vbo(vbo1, 0);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	while (!window.should_close())
	{
		window.clear();

		shader_program.activate();
		vao1.bind();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		window.flip();
		window.poll();
	}

	vao1.unload();
	vbo1.unload();
	ebo1.unload();
	shader_program.unload();

	return 0;
}
