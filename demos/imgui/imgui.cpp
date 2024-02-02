#include "Vector.hpp"
#include "Window.hpp"
#include <imgui.h>

int main(void)
{
	birb::window window("imgui test", birb::vec2<unsigned int>(960, 540));

	// Setup ImGui
	window.init_imgui();

	while (!window.should_close())
	{
		// Process inputs
		while (window.inputs_available())
		{
			// ... handle inputs here ...
			window.forget_inputs();
		}

		window.clear();

		ImGui::ShowDemoWindow();

		window.flip();
		window.poll();
	}
}
