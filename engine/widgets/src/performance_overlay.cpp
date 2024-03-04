#include "Logger.hpp"
#include "Math.hpp"
#include "PerformanceOverlay.hpp"
#include "Profiling.hpp"
#include "Window.hpp"

#include <algorithm>
#include <cassert>
#include <imgui.h>
#include <stb_sprintf.h>
#include <sys/resource.h>

namespace birb
{
	namespace overlay
	{
		// There should only be a singular performance overlay at any point
		static unsigned short perf_overlay_count = 0;

		performance::performance(timestep& ts) : ts(ts)
		{
			if (perf_overlay_count != 0)
				birb::log_warn("Spawning multiple performance overlays is unnecessary");

			perf_overlay_count++;

#ifdef BIRB_PLATFORM_LINUX
			setup_memory_history();
#endif
		}

		performance::performance(timestep& ts, const char* collapsing_menu) : collapsing_menu_name(collapsing_menu), ts(ts)
		{
			is_overlay = false;

			if (perf_overlay_count != 0)
				birb::log_warn("Spawning multiple performance overlays is unnecessary");

			perf_overlay_count++;

#ifdef BIRB_PLATFORM_LINUX
			setup_memory_history();
#endif
		}

		performance::~performance()
		{
			perf_overlay_count--;
		}

		void performance::draw()
		{
			PROFILER_SCOPE_RENDER_FN()

			float frametime_min = *std::min_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float frametime_max = *std::max_element(ts.frametime_history.begin(), ts.frametime_history.end());
			float average_frametime = birb::average(ts.frametime_history);

			assert(frametime_min != 0 && "Zero division");
			assert(frametime_max != 0 && "Zero division");
			assert(average_frametime != 0 && "Zero division");

			bool p_open = false;

			if (is_overlay)
			{
				setup_overlay();
				ImGui::Begin("Overlay", &p_open, overlay_window_flags);
				ImGui::SeparatorText("Performance");
			}
			else
			{
				ImGui::Begin("Overlays", &p_open);
			}

			if (is_overlay || ImGui::CollapsingHeader(collapsing_menu_name))
			{
				ImGui::Text("FPS: %.2f", ts.fps());
				ImGui::Text("FPS avg: %.0f", std::round(1.0f / average_frametime));
				ImGui::Text("FPS min: %.2f", 1.0f / frametime_max);
				ImGui::Text("FPS max: %.2f", 1.0f / frametime_min);

				// Frametime
				stbsp_snprintf(overlay_text_buffer.data(), overlay_text_buffer.size(), "%.5f ms", average_frametime * 1000);
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
				assert(!memory_history.empty() && "Empty memory history will cause an out-of-bounds read");
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


				ImVec4 green(0.49f, 0.72f, 0.34f, 1.0f);
				ImVec4 red(0.80f, 0.27f, 0.27f, 1.0f);

				ImGui::Text("Profiler enabled:");
				ImGui::SameLine();
#if MICROPROFILE_ENABLED == 1
				ImGui::TextColored(green, "yes");
#else
				ImGui::TextColored(red, "no");
#endif


				ImGui::Text("Debug build:");
				ImGui::SameLine();

#ifdef NDEBUG
				ImGui::TextColored(red, "no");
#else
				ImGui::TextColored(green, "yes");
#endif
			}


			ImGui::End();
		}

#ifdef BIRB_PLATFORM_LINUX
		void performance::setup_memory_history()
		{
			this->pid = RUSAGE_SELF;
			std::fill(memory_history.begin(), memory_history.end(), 0);
		}

		long performance::resident_memory_usage() const
		{
			struct rusage mem;
			getrusage(pid, &mem);
			return mem.ru_maxrss / 1024;
		}
#endif
	}
}
