#include "Vector.hpp"
#include "Window.hpp"
#include <glad/gl.h>
#include <imgui.h>

int main(void)
{
	birb::window window("imgui test", birb::vec2<int>(960, 540));

	// Setup ImGui
	window.init_imgui();

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

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