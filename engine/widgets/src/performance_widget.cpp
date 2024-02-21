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
			PROFILER_SCOPE_RENDER_FN()
			float frametime_min = *std::min_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float frametime_max = *std::max_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float average_frametime = birb::average(ts.frametime_history);

			bool p_open = false;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

			const float padding = 5.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos;
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos(work_pos.x + padding, work_pos.y + padding);
			ImVec2 window_pos_pivot(0.0f, 0.0f);

			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowBgAlpha(0.45f);

			ImGui::Begin("Performance", &p_open, window_flags);
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


#ifdef BIRB_PLATFORM_LINUX
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
#endif

			ImGui::Text("Profiler enabled:");
			ImGui::SameLine();
#if MICROPROFILE_ENABLED == 1
			ImGui::TextColored(ImVec4(0.49f, 0.72f, 0.34f, 1.0f), "yes");
#else
			ImGui::TextColored(ImVec4(0.80f, 0.27f, 0.27f, 1.0f), "no");
#endif

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
