#include "WidgetBase.hpp"
#include "Window.hpp"

#include <cassert>

namespace birb
{
	void widget_base::setup_overlay()
	{
		assert(window::imgui_is_init() && "Overlays require ImGui to be initialized");

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos;
		ImVec2 window_pos(work_pos.x + padding, work_pos.y + padding);
		ImVec2 window_pos_pivot(0.0f, 0.0f);

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(bg_alpha);
	}
}
