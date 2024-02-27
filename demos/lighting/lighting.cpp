#include "Window.hpp"
#include "Timestep.hpp"
#include "PerformanceOverlay.hpp"
#include "CameraInfoOverlay.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

int main(void)
{
	birb::window window("Lighting", birb::vec2<int>(1280, 720));
	birb::renderer renderer;
	window.init_imgui();

	birb::timestep timestep;
	birb::overlay::performance perf_widget(timestep);

	birb::camera camera({0.0f, 0.0f, -3.0f});
	window.lock_cursor_to_window();
	camera.process_input(window, timestep);
	camera.yaw = 90;
	camera.pitch = 0;

	birb::overlay::camera_info camera_widget(camera);

	const std::vector<float> cube_verts =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// Colored cube
	birb::vbo cube_vbo(cube_verts);

	birb::vao cube_vao;
	cube_vao.bind();

	cube_vao.link_vbo(cube_vbo, 0, 3, 6, 0);
	cube_vao.link_vbo(cube_vbo, 1, 3, 6, 3);

	cube_vao.unbind();
	cube_vbo.unbind();


	// Light cube
	birb::vao light_vao;
	light_vao.bind();

	light_vao.link_vbo(cube_vbo, 0, 3, 6, 0);

	light_vao.unbind();


	glm::vec3 light_pos(1.2f, 1.0f, 2.0f);


	birb::shader default_shader("default");
	default_shader.add_uniform_location(std::vector<std::string>({"model", "view", "projection"}));
	default_shader.add_uniform_location(std::vector<std::string>({"object_color", "light_color", "light_pos", "view_pos"}));

	default_shader.set_var_vec3("object_color", {1.0f, 0.5f, 0.31f});
	default_shader.set_var_vec3("light_color", {1.0f, 1.0f, 1.0f});
	default_shader.set_var_vec3("light_pos", light_pos);


	birb::shader light_shader("default", "light");
	light_shader.add_uniform_location(std::vector<std::string>({"model", "view", "projection"}));


	while (!window.should_close())
	{
		if (window.is_key_held(birb::input::keycode::UP))
			light_pos.x += timestep.deltatime();

		if (window.is_key_held(birb::input::keycode::DOWN))
			light_pos.x -= timestep.deltatime();

		if (window.is_key_held(birb::input::keycode::LEFT))
			light_pos.z += timestep.deltatime();

		if (window.is_key_held(birb::input::keycode::RIGHT))
			light_pos.z -= timestep.deltatime();

		default_shader.set_var_vec3("light_pos", light_pos);

		while (window.inputs_available())
		{
			birb::input input = window.next_input();
			switch (input.key)
			{
				case birb::input::keycode::ESCAPE:
					window.unlock_cursor_from_window();
					break;

				case birb::input::keycode::MOUSE_1:
					window.lock_cursor_to_window();
					break;

				default:
					break;
			}
		}

		camera.process_input(window, timestep);

		// Do the matrix maths
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.size().x) / static_cast<float>(window.size().y), 0.1f, 100.0f);

		default_shader.set_var_mat4("model", model);
		default_shader.set_var_mat4("view", camera.get_view_matrix());
		default_shader.set_var_mat4("projection", projection);
		default_shader.set_var_vec3("view_pos", camera.position);


		// Draw the light source cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f));

		light_shader.set_var_mat4("model", model);
		light_shader.set_var_mat4("view", camera.get_view_matrix());
		light_shader.set_var_mat4("projection", projection);


		window.clear();

		default_shader.activate();
		renderer.draw_arrays(cube_vao, cube_verts.size());

		light_shader.activate();
		renderer.draw_arrays(light_vao, cube_verts.size());

		perf_widget.draw();
		camera_widget.draw();
		window.flip();

		window.poll();
		timestep.step();
	}
}
