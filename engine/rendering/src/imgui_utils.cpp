#include "Assert.hpp"
#include "ImGUIUtils.hpp"

#include <imgui.h>

namespace birb
{
	namespace imgui
	{
		void texture(const std::string& window_name, const u32 texture_id, const vec2<i32> texture_dimensions)
		{
			ensure(!window_name.empty());
			ensure(texture_id != 0);
			ensure(texture_dimensions.x > 0);
			ensure(texture_dimensions.y > 0);

			ImVec2 window_size(texture_dimensions.x, texture_dimensions.y);

			ImGui::Begin(window_name.c_str());
			{
				ImGui::Image(static_cast<intptr_t>(texture_id), window_size, ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();
		}

		bool vec2i_slider(const std::string& window_name, const std::string& label, vec2<i32>& vec, const i32 min, const i32 max)
		{
			ensure(!window_name.empty());
			ensure(!label.empty());

			ImGui::Begin(window_name.c_str());
			const bool value_changed = ImGui::SliderInt2(label.c_str(), *vec.to_ptr_array().data(), min, max);
			ImGui::End();

			return value_changed;
		}

		bool vec3i_slider(const std::string& window_name, const std::string& label, vec3<i32>& vec, const i32 min, const i32 max)
		{
			ensure(!window_name.empty());
			ensure(!label.empty());

			ImGui::Begin(window_name.c_str());
			const bool value_changed = ImGui::SliderInt3(label.c_str(), *vec.to_ptr_array().data(), min, max);
			ImGui::End();

			return value_changed;
		}

		bool vec2f_slider(const std::string& window_name, const std::string& label, vec2<f32>& vec, const f32 min, const f32 max)
		{
			ensure(!window_name.empty());
			ensure(!label.empty());

			ImGui::Begin(window_name.c_str());
			const bool value_changed = ImGui::SliderFloat2(label.c_str(), *vec.to_ptr_array().data(), min, max);
			ImGui::End();

			return value_changed;
		}

		bool vec3f_slider(const std::string& window_name, const std::string& label, vec3<f32>& vec, const f32 min, const f32 max)
		{
			ensure(!window_name.empty());
			ensure(!label.empty());

			ImGui::Begin(window_name.c_str());
			const bool value_changed = ImGui::SliderFloat3(label.c_str(), *vec.to_ptr_array().data(), min, max);
			ImGui::End();

			return value_changed;
		}
	}
}
