#include "Math.hpp"
#include "PerformanceWidget.hpp"
#include "Profiling.hpp"

#include <algorithm>
#include <cstdio>
#include <imgui.h>
#include <stb_sprintf.h>
#include <sys/resource.h>

namespace birb
{
	namespace widget
	{
		performance::performance(timestep& ts) : ts(ts)
		{
			this->pid = RUSAGE_SELF;
			std::fill(memory_history.begin(), memory_history.end(), 0);
		}

		void performance::draw()
		{
			PROFILER_SCOPE_RENDER("Draw performance widget")
			float frametime_min = *std::min_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float frametime_max = *std::max_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float average_frametime = birb::average(ts.frametime_history);

			ImGui::Begin("Performance");
			ImGui::Text("FPS: %.2f", ts.fps());
			ImGui::Text("FPS avg: %.0f", std::round(1.0f / average_frametime));
			ImGui::Text("FPS min: %.2f", 1.0f / frametime_max);
			ImGui::Text("FPS max: %.2f", 1.0f / frametime_min);

			// Frametime
			stbsp_snprintf(overlay_text_buffer.data(), overlay_text_buffer.size(), "%.5f ms", average_frametime);
			ImGui::PlotLines("Frametime",
					ts.frametime_history.data(),
					ts.frametime_history.size(),
					0, overlay_text_buffer.data(),
					frametime_min, frametime_max + 0.005,
					ImVec2(0, 40));


			// Framebudget
			ImGui::ProgressBar(ts.framebudget(), ImVec2(0, 0));
			ImGui::SameLine(0.0f, -1.0f);
			ImGui::Text("Framebudget");


			// Memory usage
			//   Update the graph if the last value has changed
			long memory_usage = resident_memory_usage();
			if (memory_history.at(memory_history.size() - 1) != memory_usage)
			{
				std::rotate(memory_history.begin(), memory_history.begin() + 1, memory_history.end());
				memory_history.at(memory_history.size() - 1) = memory_usage;
			}

			float memory_max = *std::max_element(memory_history.begin(), memory_history.end());

			stbsp_snprintf(overlay_text_buffer.data(), overlay_text_buffer.size(), "%lu MB", memory_usage);

			ImGui::PlotHistogram("Memory usage",
					memory_history.data(),
					memory_history.size(),
					0, overlay_text_buffer.data(),
					0, memory_max + 10,
					ImVec2(0, 40));

			ImGui::End();
		}

		long performance::resident_memory_usage() const
		{
			struct rusage mem;
			getrusage(pid, &mem);
			return mem.ru_maxrss / 1024;
		}
	}
}
