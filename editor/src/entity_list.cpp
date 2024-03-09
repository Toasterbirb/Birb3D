#include "Components.hpp"
#include "EntityList.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>
#include <vector>

#ifndef NDEBUG
#include <limits>
#endif

namespace editor
{
	entity_list::entity_list(birb::scene& scene) : scene(scene)
	{
		// Create the zero separated shader list strings for combo selection windows
		const std::vector<std::string>& vert_shader_names = birb::shader::vertex_shader_name_list();
		const std::vector<std::string>& frag_shader_names = birb::shader::fragment_shader_name_list();

		assert(!vert_shader_names.empty());
		assert(!frag_shader_names.empty());

		for (const std::string& vert_shader : vert_shader_names)
			vertex_shader_name_list_str += vert_shader + '\0';

		for (const std::string& frag_shader : frag_shader_names)
			fragment_shader_name_list_str += frag_shader + '\0';

		assert(!vertex_shader_name_list_str.empty());
		assert(!fragment_shader_name_list_str.empty());

		// Find the indicies of the default shaders
		auto vert_it = std::find_if(vert_shader_names.begin(), vert_shader_names.end(), [this](const std::string& name){
			return default_vert_shader_name_str == name;
		});
		assert(vert_it != vert_shader_names.end() && "Default vertex shader doesn't exist in ShaderSource.hpp");
		assert(vert_it - vert_shader_names.begin() < std::numeric_limits<unsigned short>::max());
		default_vertex_shader_index = vert_it - vert_shader_names.begin();

		auto frag_it = std::find_if(frag_shader_names.begin(), frag_shader_names.end(), [this](const std::string& name){
			return default_frag_shader_name_str == name;
		});
		assert(frag_it != frag_shader_names.end() && "Default fragment shader doesn't exist in ShaderSource.hpp");
		assert(frag_it - frag_shader_names.begin() < std::numeric_limits<unsigned short>::max());
		default_fragment_shader_index = frag_it - frag_shader_names.begin();
	}

	entt::entity entity_list::default_entity(std::string name)
	{
		if (name.empty())
			name = "Untitled_" + std::to_string(counter++);

		duplicate_name = scene.is_duplicate_entity_info_name(name);

		if (duplicate_name)
		{
			birb::log("Duplicate entity name: " + name);
			return entt::null;
		}

		if (name == new_entity_menu_text)
			name += "_" + std::to_string(counter++);

		assert(!scene.is_duplicate_entity_info_name(name));

		entt::entity new_entity = scene.registry.create();
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
					static int selected_vert_index = default_vertex_shader_index;
					static int selected_frag_index = default_fragment_shader_index;

					ImGui::InputText("Name", &name);
					ImGui::InputText("File path", &model_file_path);

					ImGui::Combo("Vertex shader", &selected_vert_index, vertex_shader_name_list_str.c_str());
					ImGui::Combo("Fragment shader", &selected_frag_index, fragment_shader_name_list_str.c_str());

					if (ImGui::Button("Create") && !model_file_path.empty())
					{
						entt::entity entity = default_entity(name);

						if (entity != entt::null)
						{
							std::string vertex_shader = birb::shader::vertex_shader_name_list().at(selected_vert_index);
							std::string fragment_shader = birb::shader::fragment_shader_name_list().at(selected_frag_index);

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
		}
		ImGui::End();
	}
}
