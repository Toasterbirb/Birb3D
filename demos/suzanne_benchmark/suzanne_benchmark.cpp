#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "Stopwatch.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include "BoxCollider.hpp"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

int main(void)
{
	birb::window window("Suzanne benchmark", birb::vec2<int>(1280, 720));
	window.lock_cursor_to_window();
	birb::renderer renderer;
	birb::timestep timestep;
	timestep.set_target_fps(240);
	birb::camera camera;
	camera.movement_speed = 7;
	camera.far_clip = 50000;

	window.init_imgui();

	birb::overlay::performance perf_overlay(timestep);
	birb::overlay::camera_info cam_info(camera);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::scene scene;
	renderer.set_scene(scene);

	birb::model suzanne_model("suzanne.obj");

	constexpr u16 suzanne_count = 50;
	constexpr f32 position_offset = 4;
	birb::shader_ref shader_reference("default", "default_color");
	birb::shader_collection::get_shader(shader_reference)->reset_lights();
	// shader.reset_lights();

	std::array<std::array<double, suzanne_count>, suzanne_count> suzanne_entity_construction_duration_arr;

	{
		birb::stopwatch entity_timer("Entity construction");

		for (u16 i = 0; i < suzanne_count; ++i)
		{
			for (u16 j = 0; j < suzanne_count; ++j)
			{
				birb::stopwatch construction_timer;

				birb::entity suzanne = scene.create_entity();

				suzanne.add_component(suzanne_model);

				birb::transform transform;
				transform.position.x = i * position_offset;
				transform.position.y = j * position_offset;

				transform.position.z = -4.0f;
				transform.rotation.y = 45.0f;
				transform.lock();
				suzanne.add_component(transform);

				birb::collider::box box_collider(transform);
				suzanne.add_component(box_collider);

				suzanne.add_component(shader_reference);

				birb::material material({ 0.2f, 0.3f, 0.4f }, { 0.9f, 0.8f, 0.7f }, 32);
				suzanne.add_component(material);

				suzanne_entity_construction_duration_arr[i][j] = construction_timer.stop(true);
			}
		}
	}

	const double average_entity_construction_duration = birb::average(suzanne_entity_construction_duration_arr);
	std::cout << "Average entity construction: " << birb::stopwatch::format_time(average_entity_construction_duration) << "\n";

	// Reset camera rotation
	camera.process_input(window, timestep);
	camera.yaw = -90;
	camera.pitch = 0;

	while (!window.should_close())
	{
		camera.process_input(window, timestep);

		while (window.inputs_available())
		{
			birb::input input = window.next_input();

			if (input.state == birb::input::action::key_down)
			{
				switch (input.key)
				{
					case (birb::input::keycode::mouse_1):
						window.lock_cursor_to_window();
						break;

					default:
						break;
				}
			}
		}

		// glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.size().x) / static_cast<float>(window.size().y), 0.1f, 100.0f);

		window.clear();

		// Render all models
		renderer.draw_entities(camera, window.size());

		perf_overlay.draw();
		cam_info.draw();
		renderer_overlay.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
