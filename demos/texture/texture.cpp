#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>
#include <stb_image.h>

#include "PerformanceWidget.hpp"
#include "Timestep.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

int main(void)
{
	birb::window window("Texture", birb::vec2<int>(800, 800));
	birb::renderer renderer;
	birb::timestep timestep;
	birb::widget::performance performance_widget(timestep);

	window.init_imgui();

	// Format: coordinates, colors
	std::vector<float> verts =
	{
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f, // Lower right corner
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	birb::shader shader_program("texture", "texture");

	birb::vao vao1;
	vao1.bind();

	birb::vbo vbo1(verts);
	birb::ebo ebo1(indices);

	vao1.link_vbo(vbo1, 0, 3, 8, 0);
	vao1.link_vbo(vbo1, 1, 3, 8, 3);
	vao1.link_vbo(vbo1, 2, 2, 8, 6);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	unsigned int uni_id = glGetUniformLocation(shader_program.id, "scale");


	birb::texture graphic_design("texture_512.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	graphic_design.tex_unit(shader_program, "tex0", 0);


	while (!window.should_close())
	{
		window.clear();

		shader_program.activate();
		glUniform1f(uni_id, 0.5f);
		vao1.bind();

		graphic_design.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		performance_widget.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
