#include "Camera.hpp"
#include "Entity.hpp"
#include "Line.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Lines", {1280, 720});
	window.init_imgui();
	birb::scene scene;

	birb::renderer renderer;
	renderer.set_scene(scene);

	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::camera camera(window.size());

	birb::timestep timestep;

	birb::line line_l({ 0.0, 0.0, 0.0 }, { 100.0, 100.0, 0.0 });
	birb::entity line = scene.create_entity();
	line.add_component(line_l);

	while (!window.should_close())
	{
		while (window.inputs_available())
		{
			birb::input input = window.next_input();
			if (input.state == birb::input::action::key_down)
			{
				switch (input.key)
				{
					case (birb::input::keycode::mouse_1):
						window.lock_cursor_to_window();

					default:
						break;
				}
			}
		}

		window.clear();
		renderer.draw_entities(camera, window.size());
		renderer_overlay.draw();
		window.flip();

		window.poll();
		timestep.step();
	}

	return 0;
}
