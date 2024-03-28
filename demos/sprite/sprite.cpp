#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "Timestep.hpp"
#include "Transform.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Sprite", birb::vec2<int>(1280, 720));
	window.init_imgui();
	birb::timestep timestep;
	birb::camera camera;
	camera.movement_speed = 100;

	birb::scene scene;
	birb::renderer renderer;
	renderer.set_scene(scene);

	birb::overlay::renderer_overlay render_overlay(renderer);
	birb::overlay::camera_info camera_info(camera);

	{
		birb::entity entity = scene.create_entity();

		birb::sprite texture_sprite("texture_512.png", birb::color_format::RGB);
		entity.add_component(texture_sprite);

		birb::component::transform transform;
		transform.position = { 100, 100, 0 };
		transform.local_scale = { 200, 200, 1 };
		entity.add_component(transform);

		while (!window.should_close())
		{
			camera.process_input(window, timestep);
			window.forget_inputs();

			window.clear();
			renderer.draw_entities(camera.get_view_matrix(), camera.get_projection_matrix(birb::camera::projection_mode::orthographic, window.size()));
			render_overlay.draw();
			camera_info.draw();
			window.flip();

			window.poll();
			timestep.step();
		}
	}

	return 0;
}
