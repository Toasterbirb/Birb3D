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
			{
				if (ImGui::CollapsingHeader(component->collapsing_header_name().c_str()))
				{
					component->draw_editor_ui();
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					if (ImGui::Button("Delete"))
						registry.remove<T>(entity);
				}
			}
		}

		static void draw_info_table_row(const std::string& name, const std::string& data);

		template<typename T>
		static void draw_info_table_row(const std::string& name, T data)
		{
			draw_info_table_row(name, std::to_string(data));
		}

		virtual void draw_editor_ui() = 0;
		virtual std::string collapsing_header_name() const = 0;
	};
}
