#include "Renderer.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Lines", {1280, 720});
	birb::scene scene;

	birb::renderer renderer;
	renderer.set_scene(scene);

	birb::camera camera;

	birb::timestep timestep;

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
		window.flip();

		window.poll();
		timestep.step();
	}

	return 0;
}
