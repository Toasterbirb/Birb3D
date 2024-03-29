#include "Camera.hpp"
#include "Components.hpp"
#include "EntityList.hpp"
#include "Inspector.hpp"
#include "Model.hpp"
#include "Profiling.hpp"

#include <imgui.h>

namespace editor
{
	inspector::inspector(birb::scene& scene) : scene(scene)
	{
		assert(!component_names.empty());

		// Build the combo menu string for adding new components
		for (size_t i = 0; i < component_names.size(); ++i)
			component_type_str += component_names.at(i).first + '\0';
	}

	void inspector::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Inspector");
		{
			entt::entity& selected_entity = editor::entity_list::selected_entity;

			if (selected_entity != entt::null)
			{
				namespace cmp = birb::component;

				entt::registry& reg = scene.registry;

				// All entities should have the info component if they want to be visible in the entity list
				std::string previous_name = reg.get<cmp::info>(selected_entity).name;
				reg.get<cmp::info>(selected_entity).draw_editor_ui();

				// Sanity check the entity name
				if (scene.is_duplicate_entity_info_name(reg.get<cmp::info>(selected_entity).name, selected_entity))
					reg.get<cmp::info>(selected_entity).name = previous_name;

				if (reg.get<cmp::info>(selected_entity).name == entity_list::new_entity_menu_text)
					reg.get<cmp::info>(selected_entity).name += '_';

				// Draw UI components for entities in a specific order
				birb::editor_component::try_draw_ui<cmp::transform>(reg, selected_entity);
				birb::editor_component::try_draw_ui<birb::shader>(reg, selected_entity);
				birb::editor_component::try_draw_ui<cmp::material>(reg, selected_entity);
				birb::editor_component::try_draw_ui<birb::model>(reg, selected_entity);
				birb::editor_component::try_draw_ui<birb::camera>(reg, selected_entity);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				int current_component_type = -1;
				if (ImGui::Combo("Component type", &current_component_type, component_type_str.c_str()))
				{
					assert(current_component_type >= 0);
					assert(current_component_type < static_cast<int>(component_names.size()) && "Malformed component_type_str or a buffer overflow");

					// Add the new component
					switch (component_names[current_component_type].second)
					{
						case (component_type::transform):
							scene.add_component(selected_entity, birb::component::transform());
							break;

						case (component_type::material):
							scene.add_component(selected_entity, birb::component::material());
							break;
					}
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::Button("Delete entity"))
				{
					// If the entity has any meshes on it, clean those up
					auto entity_model = reg.try_get<birb::model>(selected_entity);
					if (entity_model)
						entity_model->destroy();

					reg.destroy(selected_entity);
					editor::entity_list::selected_entity = entt::null;
				}
			}
		}
		ImGui::End();
	}
}
