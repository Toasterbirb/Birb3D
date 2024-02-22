#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>

#include "Timestep.hpp"
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

int main(void)
{
	birb::window window("Triangle", birb::vec2<int>(800, 800));
	birb::renderer renderer;
	birb::timestep timestep;

	window.init_imgui();

	// Format: coordinates, colors
	std::vector<float> verts =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	0.3f, 0.3f, 0.9f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.4f, 0.1f, 0.5f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	0.3f, 0.5f, 2.7f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	0.2f, 0.2f, 0.3f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	0.0f, 0.3f, 0.7f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.9f, 0.1f, 0.7f, // Inner down
	};

	std::vector<unsigned int> indices =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1, // Upper triangle
	};

	birb::shader shader_program("color", "color");

	birb::vao vao1;
	vao1.bind();

	birb::vbo vbo1(verts);
	birb::ebo ebo1(indices);

	vao1.link_vbo(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), nullptr);
	vao1.link_vbo(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	unsigned int uni_id = glGetUniformLocation(shader_program.id, "scale");

	while (!window.should_close())
	{
		window.clear();

		shader_program.activate();
		glUniform1f(uni_id, 0.5f);
		vao1.bind();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		window.flip();
		window.poll();
		timestep.step();
	}

	vao1.unload();
	vbo1.unload();
	ebo1.unload();
	shader_program.unload();

	return 0;
}
