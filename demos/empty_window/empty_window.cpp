#include "Logger.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Empty window", birb::vec2<int>(960, 540));

	birb::timestep timestep;

	while (!window.should_close())
	{
		// Process input
		while (window.inputs_available())
		{
			birb::input cur_input = window.next_input();
			if (cur_input.key == birb::input::keycode::escape && cur_input.state == birb::input::action::key_down)
			{
				birb::log("Quitting...");
				window.quit();
			}

			if (cur_input.key == birb::input::keycode::mouse_left && cur_input.state == birb::input::action::key_down)
				birb::log("Mouse click at " + cur_input.pos.to_string());
		}

		window.clear();
		window.flip();
		window.poll();

		timestep.step();
	}
	return 0;
}
