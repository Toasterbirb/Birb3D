#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
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

	window.lock_cursor_to_window();

	window.init_imgui();

	// Format: coordinates, colors
	const std::vector<float> verts =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	birb::shader shader_program("texture");

	birb::vao vao1;
	vao1.bind();

	birb::vbo vbo1(verts);
	birb::ebo ebo1(indices);

	vao1.link_vbo(vbo1, 0, 3, 5, 0);
	vao1.link_vbo(vbo1, 1, 2, 5, 3);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();


	std::vector<float> plane_verts =
	{
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,

		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
	};
	birb::vao plane_vao;
	plane_vao.bind();

	birb::vbo plane_vbo(plane_verts);
	plane_vao.link_vbo(plane_vbo, 0, 3, 3, 0);
	plane_vao.unbind();
	plane_vbo.unbind();



	shader_program.add_uniform_location("model");
	shader_program.add_uniform_location("view");
	shader_program.add_uniform_location("projection");


	birb::texture graphic_design("texture_512.png", 0, birb::color_format::RGB);
	graphic_design.tex_unit(shader_program);


	/** Camera stuff **/
	birb::camera camera({0.0f, 0.0f, 3.0f});


	birb::vec3<float> position;

	while (!window.should_close())
	{
		camera.process_input(window, timestep);
		window.forget_inputs();

		window.clear();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(static_cast<float>(timestep.time_since_startup()) * rotation_speed), glm::vec3(0.7f, 1.0f, 0.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));


		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.size().x) / static_cast<float>(window.size().y), 0.1f, 100.0f);

		shader_program.activate();
		shader_program.set_var_mat4("model", model);
		shader_program.set_var_mat4("view", camera.get_view_matrix());
		shader_program.set_var_mat4("projection", projection);
		vao1.bind();

		graphic_design.bind();
		// renderer.draw_elements(vao1, indices.size());
		renderer.draw_arrays(vao1, verts.size());


		// Render the floor
		glm::mat4 plane_model = glm::mat4(1.0f);
		plane_model = glm::translate(plane_model, glm::vec3(0.0f, -0.5f, 0.0f));
		plane_model = glm::rotate(plane_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		shader_program.set_var_mat4("model", plane_model);
		renderer.draw_arrays(plane_vao, plane_verts.size());

		performance_widget.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
