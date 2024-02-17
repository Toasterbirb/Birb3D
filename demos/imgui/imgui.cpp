#include "PerformanceWidget.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <glad/gl.h>
#include <imgui.h>

int main(void)
{
	birb::window window("imgui test", birb::vec2<int>(960, 540));

	birb::timestep timestep;
	birb::widget::performance perf_widget(timestep);

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
		perf_widget.draw();

		window.flip();
		window.poll();

		timestep.step();
	}
}
