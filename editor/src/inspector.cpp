#include "Components.hpp"
#include "EntityList.hpp"
#include "Inspector.hpp"
#include "Model.hpp"
#include "Profiling.hpp"

#include <imgui.h>

namespace editor
{
	inspector::inspector(birb::scene& scene) : scene(scene) {}

	void inspector::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Inspector");
		{
			const entt::entity& selected_entity = editor::entity_list::selected_entity;

			if (selected_entity != entt::null)
			{
				namespace cmp = birb::component;

				entt::registry& reg = scene.get_registry();

				// All entities should have the info component if they want to be visible in the entity list
				reg.get<cmp::info>(selected_entity).draw_editor_ui();

				// Sanity check the entity name
				if (reg.get<cmp::info>(selected_entity).name == entity_list::new_entity_button_text)
					reg.get<cmp::info>(selected_entity).name += '_';

				// Draw UI components for entities in a specific order
				birb::editor_component::try_draw_ui<cmp::transform>(reg, selected_entity);
				birb::editor_component::try_draw_ui<birb::shader>(reg, selected_entity);
				birb::editor_component::try_draw_ui<birb::model>(reg, selected_entity);

				ImGui::Separator();

				int current_component_type = -1;
				ImGui::Combo("Component type", &current_component_type, "Hello\0World\0");
			}
		}
		ImGui::End();
	}
}
