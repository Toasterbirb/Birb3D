#pragma once

#include <entt.hpp>
#include <imgui.h>
#include <string>

namespace birb
{
	/**
	 * @class editor_component
	 * @brief Interface for adding editor UI to entity components
	 *
	 */
	class editor_component
	{
	public:
		template<typename T>
		static void try_draw_ui(entt::registry& registry, const entt::entity& entity)
		{
			if (T* component = registry.try_get<T>(entity))
				component->draw_editor_ui();
		}

		static void draw_info_table_row(const std::string& name, const std::string& data)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", name.c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%s", data.c_str());
		}

		template<typename T>
		static void draw_info_table_row(const std::string& name, T data)
		{
			draw_info_table_row(name, std::to_string(data));
		}

		virtual void draw_editor_ui() = 0;
	};
}
