#include "Assert.hpp"
#include "ImGUIUtils.hpp"

#include <imgui.h>

namespace birb
{
	namespace imgui_utils
	{
		void draw_texture(const std::string window_name, const u32 texture_id, const vec2<i32> texture_dimensions)
		{
			ensure(!window_name.empty());
			ensure(texture_id != 0);
			ensure(texture_dimensions.x > 0);
			ensure(texture_dimensions.y > 0);

			ImVec2 window_size(texture_dimensions.x, texture_dimensions.y);

			ImGui::Begin(window_name.c_str());
			{
				ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(texture_id)), window_size, ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();
		}
	}
}
