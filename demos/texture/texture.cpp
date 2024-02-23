#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // Lower right corner
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

	vao1.link_vbo(vbo1, 0, 3, 5, 0);
	vao1.link_vbo(vbo1, 1, 2, 5, 3);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	shader_program.add_uniform_location("transform");


	birb::texture graphic_design("texture_512.png", 0, birb::color_format::RGB);
	graphic_design.tex_unit(shader_program, "tex0", 0);


	constexpr float rotation_speed = 5.0f;

	while (!window.should_close())
	{
		window.clear();

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.2f, 0.2f, 0.0f));
		trans = glm::rotate(trans, glm::radians(static_cast<float>(timestep.time_since_startup()) * rotation_speed), glm::vec3(0.0, 0.0, 1.0f));
		trans = glm::scale(trans, glm::vec3(1.2f, 1.2f, 1.2f));

		shader_program.activate();
		shader_program.set_var_mat4("transform", trans);
		vao1.bind();

		graphic_design.bind();
		renderer.draw_elements(vao1, indices.size());

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
		shader_program.set_var_mat4("transform", trans);
		renderer.draw_elements(vao1, indices.size());

		performance_widget.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
