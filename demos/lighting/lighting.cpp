#include "Window.hpp"
#include "Timestep.hpp"
#include "PerformanceOverlay.hpp"
#include "CameraInfoOverlay.hpp"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Profiling.hpp"
#include "Camera.hpp"
#include "TextureMaterial.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

static birb::random rng;

static void randomize_positions(std::vector<glm::vec3>& cube_pos_vec)
{
	constexpr float position_range = 20.0f;

	for (size_t i = 0; i < cube_pos_vec.size(); ++i)
	{
		float x = rng.range_float(-position_range, position_range);
		float y = rng.range_float(-position_range, position_range);
		float z = rng.range_float(-position_range, position_range);

		cube_pos_vec[i] = {x, y, z};
	}
}

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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// Colored cube
	birb::vbo cube_vbo(cube_verts);

	birb::vao cube_vao;
	cube_vao.bind();

	cube_vao.link_vbo(cube_vbo, 0, 3, 8, 0);
	cube_vao.link_vbo(cube_vbo, 1, 3, 8, 3);
	cube_vao.link_vbo(cube_vbo, 2, 2, 8, 6);

	cube_vao.unbind();
	cube_vbo.unbind();


	// Light cube
	birb::vao light_vao;
	light_vao.bind();

	light_vao.link_vbo(cube_vbo, 0, 3, 8, 0);

	light_vao.unbind();


	std::array<birb::vec3<float>, 4> light_positions;
	std::array<birb::vec3<float>, light_positions.size()> light_colors;
	for (size_t i = 0; i < light_positions.size(); ++i)
	{
		float x = rng.range_float(-5.0f, 5.0f);
		float y = rng.range_float(-5.0f, 5.0f);
		float z = rng.range_float(-5.0f, 5.0f);
		light_positions[i] = {x, y, z};

		float r = rng.range_float(0.0f, 1.0f);
		float g = rng.range_float(0.0f, 1.0f);
		float b = rng.range_float(0.0f, 1.0f);
		light_colors[i] = {r, g, b};
	}

	birb::shader default_shader("default");
	default_shader.reset_lights();

	// Point light
	for (size_t i = 0; i < light_positions.size(); ++i)
	{
		default_shader.point_lights[i].ambient = {0.2f, 0.2f, 0.2f};
		default_shader.point_lights[i].diffuse = light_colors[i];
		default_shader.point_lights[i].specular = {1.0f, 1.0f, 1.0f};

		default_shader.point_lights[i].position = light_positions[i];

		default_shader.point_lights[i].attenuation_constant = 1.0f;
		default_shader.point_lights[i].attenuation_linear = 0.09f;
		default_shader.point_lights[i].attenuation_quadratic = 0.032f;
	}

	default_shader.update_point_lights();
	default_shader.update_directional_light();

	birb::texture_material material("texture_512.png", "specular_512.png", 32);
	material.apply_to_shader(default_shader);

	birb::shader light_shader("default", "light");

	constexpr int cube_count = 1000;
	std::vector<glm::vec3> cube_positions(cube_count);
	randomize_positions(cube_positions);

	while (!window.should_close())
	{
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

			if (input.state == birb::input::action::KEY_DOWN)
			{
				switch (input.key)
				{
					case birb::input::keycode::R:
						randomize_positions(cube_positions);
						break;

					default:
						break;
				}
			}
		}

		camera.process_input(window, timestep);

		window.clear();

		default_shader.activate();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.size().x) / static_cast<float>(window.size().y), 0.1f, 100.0f);
		default_shader.set(birb::shader_uniforms::view, camera.get_view_matrix());
		default_shader.set(birb::shader_uniforms::projection, projection);
		default_shader.set(birb::shader_uniforms::view_pos, camera.position);

		// Do the matrix maths
		std::vector<glm::mat4> cube_model_matrices(cube_positions.size());

		std::transform(std::execution::par, cube_positions.begin(), cube_positions.end(), cube_model_matrices.begin(), [timestep](const glm::vec3& position) {
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(((position.x + position.y + position.z) * 20.0f) + static_cast<float>(timestep.time_since_startup()) * 50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

			return model;
		});

		for (size_t i = 0; i < cube_positions.size(); ++i)
		{
			default_shader.set(birb::shader_uniforms::model, cube_model_matrices[i]);
			renderer.draw_arrays(cube_vao, cube_verts.size());
		}

		light_shader.set(birb::shader_uniforms::view, camera.get_view_matrix());
		light_shader.set(birb::shader_uniforms::projection, projection);

		light_shader.activate();

		// Draw the light source cubes
		{
			PROFILER_SCOPE_RENDER("Render light sources")
			for (size_t i = 0; i < light_positions.size(); ++i)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, light_positions[i].to_glm_vec());
				model = glm::scale(model, glm::vec3(0.2f));

				light_shader.set(birb::shader_uniforms::model, model);
				light_shader.set(birb::shader_uniforms::color, birb::color(light_colors[i].x, light_colors[i].y, light_colors[i].z));
				renderer.draw_arrays(light_vao, cube_verts.size());
			}
		}

		perf_widget.draw();
		camera_widget.draw();
		window.flip();

		window.poll();
		timestep.step();
	}
}
