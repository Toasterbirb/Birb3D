#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Entity.hpp"
#include "PerformanceOverlay.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "Timestep.hpp"
#include "Transform.hpp"
#include "Window.hpp"

#include <algorithm>

int main(void)
{
	birb::window window("Sprite", birb::vec2<int>(1280, 720));
	window.init_imgui();
	birb::timestep timestep;
	birb::camera camera;
	camera.movement_speed = 100;
	camera.position.z = 45; // Move the camera back to make orthograhpics rendering work
	camera.orthograhpic_scale = 0.5f;

	birb::scene scene;
	birb::renderer renderer;
	renderer.set_scene(scene);

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay render_overlay(renderer);
	birb::overlay::camera_info camera_info(camera);

	{
		birb::sprite texture_sprite("texture_512.png", birb::color_format::RGB);

		birb::sprite wide_texture_sprite("wide_texture_512x256.png", birb::color_format::RGB);
		wide_texture_sprite.ignore_aspect_ratio = false;

		birb::entity entity = scene.create_entity();
		birb::entity second_entity = scene.create_entity();

		// Setup the first entity
		{
			birb::component::transform transform;
			transform.position = { 100, 0, 0 };
			transform.local_scale = { 100, 100, 1 };
			entity.add_component(transform);
			entity.add_component(texture_sprite);
		}

		// Setup the second entity
		{
			birb::component::transform transform;
			transform.position = { 100, 100.0f / 2.0f + 40.0f / 2.0f, 0 };
			transform.local_scale = { 40, 40, 1 };
			second_entity.add_component(transform);
			second_entity.add_component(texture_sprite);
		}

		// Copy pasted entities
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				birb::entity ent = scene.create_entity();

				birb::component::transform transform;
				transform.position = { i * 16.0f + 200, j * 16.0f, 1.0f };
				transform.local_scale = { 8, 8, 1 };
				transform.lock();

				ent.add_component(transform);
				ent.add_component(texture_sprite);
			}
		}

		// Wide sprite
		{
			birb::entity wide = scene.create_entity();

			birb::component::transform transform;
			transform.position = { 140, 100.0f / 2.0f + 40.0f / 2.0f, 2.0f };
			transform.local_scale = { 40.0f, 40.0f, 1.0f };
			transform.rotation.z = 45;

			wide.add_component(transform);
			wide.add_component(wide_texture_sprite);
		}

		// Zooming thingie
		float timer = 0.0f;
		bool zoom_done = false;

		while (!window.should_close())
		{
			// Cool zoom thing
			if (camera.orthograhpic_scale > 0.2f && !zoom_done)
			{
				camera.orthograhpic_scale = std::lerp(camera.orthograhpic_scale, 0.2f, timer);
				timer += timestep.deltatime();
			}
			else
			{
				zoom_done = true;
			}

			while (window.inputs_available())
			{
				birb::input input = window.next_input();

				switch (input.key)
				{
					case (birb::input::keycode::SCROLLING):
						camera.orthograhpic_scale -= input.offset.y * 0.01;
						camera.orthograhpic_scale = std::clamp(camera.orthograhpic_scale, 0.001f, 4096.0f);
						break;

					default:
						break;
				}
			}

			// WASD movement with orthographic controls
			camera.process_input_ortho(window, timestep);

			window.forget_inputs();

			window.clear();
			renderer.draw_entities(camera.get_view_matrix(), camera.get_projection_matrix(birb::camera::projection_mode::orthographic, window.size()));
			performance_overlay.draw();
			render_overlay.draw();
			camera_info.draw();
			window.flip();

			window.poll();
			timestep.step();
		}
	}

	return 0;
}
