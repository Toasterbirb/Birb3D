#include "PerformanceWidget.hpp"

#include <imgui.h>
#include <sys/resource.h>


namespace birb
{
	performance_widget::performance_widget()
	{
		this->pid = RUSAGE_SELF;
	}

	void performance_widget::draw()
	{
		ImGui::Begin("Performance");

		ImGui::Text("FPS: %d", fps());
		ImGui::Text("Frametime: %.2f", -1.00f);
		ImGui::Text("Framebudget: %.2f%%", -1.00f);
		ImGui::Text("Memory: %lumb", resident_memory_usage());

		ImGui::End();
	}

	int performance_widget::fps() const
	{
		return -1.0f;
	}

	long performance_widget::resident_memory_usage() const
	{
		struct rusage mem;
		getrusage(pid, &mem);
		return mem.ru_maxrss / 1024;
	}
}
