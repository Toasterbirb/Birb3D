#include "Camera.hpp"
#include "Components.hpp"
#include "EntityList.hpp"
#include "Logger.hpp"
#include "Model.hpp"
#include "PrimitiveMeshes.hpp"
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

		birb::ensure(!vert_shader_names.empty());
		birb::ensure(!frag_shader_names.empty());

		for (const std::string& vert_shader : vert_shader_names)
			vertex_shader_name_list_str += vert_shader + '\0';

		for (const std::string& frag_shader : frag_shader_names)
			fragment_shader_name_list_str += frag_shader + '\0';

		birb::ensure(!vertex_shader_name_list_str.empty());
		birb::ensure(!fragment_shader_name_list_str.empty());

		// Find the indicies of the default shaders
		auto vert_it = std::find_if(vert_shader_names.begin(), vert_shader_names.end(), [this](const std::string& name){
			return default_vert_shader_name_str == name;
		});
		birb::ensure(vert_it != vert_shader_names.end() && "Default vertex shader doesn't exist in ShaderSource.hpp");
		birb::ensure(vert_it - vert_shader_names.begin() < std::numeric_limits<u16>::max());
		default_vertex_shader_index = vert_it - vert_shader_names.begin();

		auto frag_it = std::find_if(frag_shader_names.begin(), frag_shader_names.end(), [this](const std::string& name){
			return default_frag_shader_name_str == name;
		});
		birb::ensure(frag_it != frag_shader_names.end() && "Default fragment shader doesn't exist in ShaderSource.hpp");
		birb::ensure(frag_it - frag_shader_names.begin() < std::numeric_limits<u16>::max());
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

		birb::ensure(!scene.is_duplicate_entity_info_name(name));

		entt::entity new_entity = scene.registry.create();
		birb::info default_info(name);
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
					static i32 selected_vert_index = default_vertex_shader_index;
					static i32 selected_frag_index = default_fragment_shader_index;

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
							shader.reset_lights();
							scene.add_component(entity, shader);

							birb::color diffuse(rng.range_float(0.0f, 1.0f), rng.range_float(0.0f, 1.0f), rng.range_float(0.0f, 1.0f));
							birb::color specular(1.0f, 1.0f, 1.0f);
							birb::material material(diffuse, specular);
							scene.add_component(entity, material);

							birb::transform transform;
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

				if (ImGui::BeginMenu("Camera"))
				{
					ImGui::InputText("Name", &name);

					if (ImGui::Button("Create"))
					{
						entt::entity entity = default_entity(name);

						if (entity != entt::null)
						{
							birb::camera camera;
							scene.add_component(entity, camera);

							birb::transform camera_transform;
							camera_transform.local_scale = {0.2, 0.2, 0.2};
							scene.add_component(entity, camera_transform);

							birb::model camera_model;
							camera_model.load_model_from_memory(birb::primitive_mesh::camera, "Camera");
							scene.add_component(entity, camera_model);

							birb::shader camera_shader("default", "default_color");

							birb::color diffuse(1.0f, 0.619608f, 0.231373f, 1.0f);
							birb::color specular(0.121569f, 0.121569f, 0.156863f, 1.0f);
							birb::material camera_material(diffuse, specular);
							scene.add_component(entity, camera_material);

							scene.add_component(entity, camera_shader);

							name.clear();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

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
		}
		ImGui::End();
	}
}
