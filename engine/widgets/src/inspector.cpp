#include "Assert.hpp"
#include "BoxCollider.hpp"
#include "Camera.hpp"
#include "EntityEditor.hpp"
#include "Info.hpp"
#include "Inspector.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Rigidbody.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "Transform.hpp"

#include <imgui.h>

// macro for drawing component UIs into the inspector
#define cmp_ui(cmp_type) birb::editor_component::try_draw_ui<cmp_type>(reg, selected_entity)

namespace birb
{
	inspector::inspector(birb::scene& scene) : scene(scene)
	{
		ensure(!component_names.empty());

		// Build the combo menu string for adding new components
		for (size_t i = 0; i < component_names.size(); ++i)
			component_type_str += component_names.at(i).first + '\0';
	}

	void inspector::set_selected(entt::entity& entity)
	{
		selected_entity = entity;
	}

	void inspector::draw()
	{
		PROFILER_SCOPE_RENDER_FN();

		ImGui::SetNextWindowSize(ImVec2(256, 128), ImGuiCond_FirstUseEver);

		ImGui::Begin(window_name);
		{
			if (selected_entity != entt::null)
			{
				entt::registry& reg = scene.registry;

				// All entities should have the info component if they want to be visible in the entity list
				std::string previous_name = reg.get<info>(selected_entity).name;
				reg.get<info>(selected_entity).draw_editor_ui();

				// Sanity check the entity name
				if (scene.is_duplicate_entity_info_name(reg.get<info>(selected_entity).name, selected_entity))
					reg.get<info>(selected_entity).name = previous_name;

				// if (reg.get<info>(selected_entity).name == entity_list::new_entity_menu_text)
				// 	reg.get<info>(selected_entity).name += '_';

				// Draw UI components for entities in a specific order
				cmp_ui(transform);
				cmp_ui(rigidbody);
				cmp_ui(collider::box);
				cmp_ui(shader);
				cmp_ui(material);
				cmp_ui(model);
				cmp_ui(camera);

				// shader references need special treatment since the class is difficult
				// to inherit from the editor_component class
				if (shader_ref* ref = reg.try_get<shader_ref>(selected_entity))
				{
					if (ImGui::CollapsingHeader("Shader (r)"))
					{
						const std::shared_ptr<shader> shader = shader_collection::get_shader(*ref);
						shader->draw_editor_ui();
					}
				}


				// Draw only editor specific stuff after this point
#ifdef BIRB_EDITOR
				if (!editor_mode)
					return;

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				i32 current_component_type = -1;
				if (ImGui::Combo("Component type", &current_component_type, component_type_str.c_str()))
				{
					ensure(current_component_type >= 0);
					ensure(current_component_type < static_cast<i32>(component_names.size()), "Malformed component_type_str or a buffer overflow");

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
#endif
			}
		}
		ImGui::End();
	}
}
