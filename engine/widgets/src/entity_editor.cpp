#include "EntityEditor.hpp"
#include "Info.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Scene.hpp"

#include <imgui.h>

namespace birb
{
	entity_editor::entity_editor(birb::scene& scene)
	:inspector(scene), scene(scene)
	{}

	void entity_editor::draw()
	{
		PROFILER_SCOPE_RENDER_FN();

		ImGui::SetNextWindowSize(ImVec2(256, 128), ImGuiCond_FirstUseEver);

		ImGui::Begin(window_name);
		{
			const auto view = scene.registry.view<birb::info>();

			for (const auto entity : view)
			{
				const birb::info& info = view.get<birb::info>(entity);

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
