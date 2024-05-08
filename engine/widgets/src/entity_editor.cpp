#include "EntityEditor.hpp"
#include "Info.hpp"
#include "Profiling.hpp"
#include "Scene.hpp"

#include <imgui.h>

namespace birb
{
	entity_editor::entity_editor(birb::scene& scene)
	:scene(scene), inspector(scene)
	{}

	void entity_editor::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Entity editor");
		{
			const auto view = scene.registry.view<birb::component::info>();

			for (const auto entity : view)
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

			inspector.set_selected(selected_entity);
			inspector.draw();
		}
		ImGui::End();
	}
}
