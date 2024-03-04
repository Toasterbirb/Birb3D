#include "Components.hpp"
#include "EntityList.hpp"
#include "Scene.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace editor
{
	entity_list::entity_list(birb::scene& scene) : scene(scene) {}

	void entity_list::draw()
	{
		ImGui::Begin("Entities");
		{
			if (ImGui::Button(new_entity_button_text.c_str()))
			{
				entt::entity new_entity = scene.get_registry().create();
				birb::component::info default_info("Untitled_" + std::to_string(counter++));
				scene.add_component(new_entity, default_info);
			}

			auto view = scene.get_registry().view<birb::component::info>();

			for (auto entity : view)
			{
				const birb::component::info& info = view.get<birb::component::info>(entity);

				std::string entity_name = info.name;
				if (entity_name.empty())
					entity_name = "Untitled";

				if (ImGui::Selectable(entity_name.c_str(), entity == selected_entity))
				{
					selected_entity = entity;
					birb::log("Selected: " + info.name);
				}
			}
		}
		ImGui::End();
	}
}
