#include "Profiling.hpp"
#include "WindowInfoOverlay.hpp"

namespace birb
{
	namespace overlay
	{
		window_info::window_info(birb::window& window) : window(window)
		{
			is_overlay = true;
		}

		window_info::window_info(birb::window& window, const char* collapsing_menu)
		:window(window), collapsing_menu_name(collapsing_menu)
		{
			is_overlay = false;
		}

		void window_info::draw()
		{
			PROFILER_SCOPE_RENDER_FN()

			if (is_overlay)
			{
				setup_overlay();
				ImGui::Begin("Overlay", &p_open, overlay_window_flags);
				ImGui::SeparatorText("Window");
			}
			else
			{
				ImGui::Begin(overlay_window_name, &p_open);
			}

			if (is_overlay || ImGui::CollapsingHeader(collapsing_menu_name))
			{
				ImGui::Text("Window size: [%d, %d]", window.size().x, window.size().y);
				ImGui::Text("Monitor refreshrate: %d", window.monitor_refreshrate());

				birb::vec2<double> cursor_pos = window.cursor_pos();
				ImGui::Text("Cursor position: [%.1lf, %.1lf]", cursor_pos.x, cursor_pos.y);
			}

			ImGui::End();
		}
	}
}
