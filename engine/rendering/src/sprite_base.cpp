#include "SpriteBase.hpp"

#include <imgui.h>

namespace birb
{
	void sprite_base::draw_editor_base_ui()
	{
		ImGui::Checkbox("Ignore aspect ratio", &ignore_aspect_ratio);
		ImGui::Checkbox("Orthographic projection", &orthographic_projection);

		static int current_ratio_lock_mode = 1; // default to height
		ImGui::SetNextItemWidth(ImGui::CalcItemWidth() / 2);
		if (ImGui::Combo("Aspect ratio lock", &current_ratio_lock_mode, "width\0height\0"))
		{
			aspect_ratio_lock = static_cast<enum aspect_ratio_lock>(current_ratio_lock_mode);
		}
	}
}
