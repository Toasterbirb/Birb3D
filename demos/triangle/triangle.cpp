#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>

#include "PerformanceWidget.hpp"
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
	birb::widget::performance perf_widget(timestep);

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

	vao1.link_vbo(vbo1, 0, 3, 6, 0);
	vao1.link_vbo(vbo1, 1, 3, 6, 3);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	shader_program.add_uniform_location("scale");

	while (!window.should_close())
	{
		window.clear();

		shader_program.activate();
		shader_program.set_var("scale", 0.5f);
		vao1.bind();

		renderer.draw_elements(vao1, indices.size());

		perf_widget.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
