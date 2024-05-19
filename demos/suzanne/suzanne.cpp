#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Info.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include "BoxCollider.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

int main(void)
{
	birb::window window("Suzanne", birb::vec2<int>(1280, 720));
	window.lock_cursor_to_window();
	birb::renderer renderer;
	birb::timestep timestep;
	birb::camera camera;
	camera.movement_speed = 7;

	window.init_imgui();

	birb::overlay::performance perf_overlay(timestep);
	birb::overlay::camera_info cam_info(camera);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::scene scene;
	renderer.set_scene(scene);

	// Allocate debug windows
	renderer.debug.alloc_world(window);
	renderer.debug.alloc_entity_editor(scene);

	birb::entity suzanne = scene.create_entity();

	birb::info suzanne_info;
	suzanne_info.name = "Suzanne";
	suzanne.add_component(suzanne_info);

	birb::model suzanne_model("suzanne.obj", "suzanne.mtl");
	suzanne.add_component(suzanne_model);

	birb::transform transform;
	transform.position.z = -4.0f;
	transform.rotation.y = 45.0f;
	suzanne.add_component(transform);

	birb::collider::box box_collider;
	box_collider.set_position_and_size(transform);
	suzanne.add_component(box_collider);

	birb::shader_ref shader("default", "default_color");
	suzanne.add_component(shader);

	// birb::material material({ 0.2f, 0.3f, 0.4f }, { 0.9f, 0.8f, 0.7f }, 32);
	// suzanne.add_component(material);

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
						// window.lock_cursor_to_window();
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
