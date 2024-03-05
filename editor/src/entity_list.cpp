#include "Components.hpp"
#include "EntityList.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace editor
{
	entity_list::entity_list(birb::scene& scene) : scene(scene) {}

	entt::entity entity_list::default_entity(std::string name)
	{
		duplicate_name = scene.is_duplicate_entity_info_name(name);

		if (duplicate_name)
		{
			birb::log("Duplicate entity name: " + name);
			return entt::null;
		}

		if (name.empty())
			name = "Untitled_" + std::to_string(counter++);

		if (name == new_entity_menu_text)
			name += "_" + std::to_string(counter++);

		entt::entity new_entity = scene.get_registry().create();
		birb::component::info default_info(name);
		scene.add_component(new_entity, default_info);

		return new_entity;
	}

	void entity_list::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Entities");
		{
			if (ImGui::BeginMenu("New entity"))
			{
				static std::string name;

				// Empty entity with no other components than the info component
				if (ImGui::BeginMenu("Empty"))
				{
					ImGui::InputText("Name", &name);
					if (ImGui::Button("Create"))
					{
						default_entity(name);
						name.clear();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Model"))
				{
					static std::string model_file_path;
					static std::string vertex_shader = "default";
					static std::string fragment_shader = "default_color";

					ImGui::InputText("Name", &name);
					ImGui::InputText("File path", &model_file_path);
					ImGui::InputText("Vertex shader", &vertex_shader);
					ImGui::InputText("Fragment shader", &fragment_shader);

					if (ImGui::Button("Create"))
					{
						entt::entity entity = default_entity(name);

						if (entity != entt::null)
						{
							birb::shader shader(vertex_shader, fragment_shader);
							birb::color diffuse(rng.range_float(0.0f, 1.0f), rng.range_float(0.0f, 1.0f), rng.range_float(0.0f, 1.0f));
							birb::color specular(1.0f, 1.0f, 1.0f);
							shader.reset_lights();
							shader.set_diffuse_color(diffuse);
							shader.set_specular_color(specular);
							shader.set_shininess(32);
							shader.add_default_3d_matrix_uniforms();
							scene.add_component(entity, shader);

							birb::component::transform transform;
							scene.add_component(entity, transform);

							birb::model model(model_file_path);
							scene.add_component(entity, model);

							// Clear some of the fields
							name.clear();
							model_file_path.clear();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

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
