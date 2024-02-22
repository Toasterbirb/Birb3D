#include "Window.hpp"
#include "Timestep.hpp"
#include "PerformanceWidget.hpp"

int main(void)
{
	birb::window window("Playground", birb::vec2<int>(1280, 720));
	window.init_imgui();

	birb::timestep timestep;
	birb::widget::performance perf_widget(timestep);

	while (!window.should_close())
	{
		window.clear();
		// Draw stuff here
		perf_widget.draw();
		window.flip();

		window.poll();
		timestep.step();
	}
}
