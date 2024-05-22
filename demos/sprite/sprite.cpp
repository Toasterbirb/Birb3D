#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Entity.hpp"
#include "PerformanceOverlay.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "Timestep.hpp"
#include "Transform.hpp"
#include "Transformer.hpp"
#include "Types.hpp"
#include "Window.hpp"

#include <algorithm>

int main(void)
{
	birb::window window("Sprite", birb::vec2<i32>(1280, 720));
	window.init_imgui();
	birb::timestep timestep(120);
	birb::camera camera;
	camera.movement_speed = 100;
	camera.position.z = 45; // Move the camera back to make orthograhpics rendering work
	camera.orthograhpic_scale = 0.5f;

	birb::scene scene;
	birb::renderer renderer;
	renderer.set_scene(scene);
	renderer.set_window(window);
	renderer.opt_post_process(true);
	renderer.opt_gamma_correction(false);

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
			birb::transform transform;
			transform.position = { 100, 0, 0 };
			transform.local_scale = { 100, 100, 1 };
			entity.add_component(transform);
			entity.add_component(texture_sprite);
		}

		// Setup the second entity
		{
			birb::transform transform;
			transform.position = { 100, 100.0f / 2.0f + 40.0f / 2.0f, 0 };
			transform.local_scale = { 40, 40, 1 };
			second_entity.add_component(transform);
			second_entity.add_component(texture_sprite);
		}

		// Copy pasted entities
		birb::entity transformer_entity = scene.create_entity();
		transformer_entity.add_component(texture_sprite);

		birb::transformer transformer;
		for (u32 i = 0; i < 128; ++i)
		{
			for (u32 j = 0; j < 128; ++j)
			{
				birb::transform transform;
				transform.position = { i * 16.0f + 200, j * 16.0f, 1.0f };
				transform.local_scale = { 8, 8, 1 };
				transformer.transforms.push_back(transform);
			}
		}
		transformer.lock();
		transformer_entity.add_component(transformer);

		// Wide sprite
		{
			birb::entity wide = scene.create_entity();

			birb::transform transform;
			transform.position = { 140, 100.0f / 2.0f + 40.0f / 2.0f, 2.0f };
			transform.local_scale = { 40.0f, 40.0f, 1.0f };
			transform.rotation.z = 45;

			wide.add_component(transform);
			wide.add_component(wide_texture_sprite);
		}

		// Zooming thingie
		f32 timer = 0.0f;
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
					case (birb::input::keycode::scrolling):
						camera.orthograhpic_scale -= input.offset.y * 0.02;
						camera.orthograhpic_scale = std::clamp(camera.orthograhpic_scale, 0.001f, 4096.0f);
						break;

					default:
						break;
				}
			}

			birb::transformer& transformer = transformer_entity.get_component<birb::transformer>();
			transformer.transforms[0].rotation.z += timestep.deltatime() * 10.0f;
			transformer.update_transform(0);
			transformer.update_vbo_data();


			// WASD movement with orthographic controls
			camera.process_input_ortho(window, timestep);

			window.forget_inputs();

			window.clear();
			renderer.draw_entities(camera, window.size());
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
