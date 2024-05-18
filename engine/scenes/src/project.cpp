#include "Assert.hpp"
#include "Components.hpp"
#include "Globals.hpp"
#include "IO.hpp"
#include "Logger.hpp"
#include "Model.hpp"
#include "Project.hpp"
#include "ProjectJsonKeys.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <entt.hpp>
#include <filesystem>
#include <fstream>
#include <string>

namespace birb
{
	project::project()
	{
		project_json = project::default_project();
	}

	project::project(const std::string& path)
	:file_path(path)
	{
		ensure(!path.empty());

		load(path);
	}

	void project::load(const std::string& path)
	{
		ensure(!path.empty());

		file_path = path;

		// Only load the project if the file exists
		// Otherwise create a new default project
		if (std::filesystem::exists(path))
		{
			const std::string& json_str = io::read_file(path);
			project_json =  nlohmann::json::parse(json_str);
			load_project();
		}
		else
		{
			birb::log_warn("No project file was found at " + path);
			project_json = default_project();
		}
	}

	void project::save(const camera& camera)
	{
		save_camera_settings(camera);
		save();
	}

	void project::save()
	{
		birb::log("Saving the project to " + file_path);

		// Store the project with the current version format
		project_json[keys::version] = version;

		// Loop through scenes and save their entities
		for (std::string scene_name : project_json[keys::scene_name_list])
		{
			// Get rid of unused components so that the loading of the project
			// doesn't fail if there were deleted entities
			scene_collection.at(scene_name).registry.compact();

			std::ofstream entity_archive(scene_name + entity_data_file_extension, std::ios::binary);
			cereal::BinaryOutputArchive archive(entity_archive);

			entt::snapshot{scene_collection.at(scene_name).registry}
				.get<entt::entity>(archive)
				.get<info>(archive)
				.get<transform>(archive)
				.get<material>(archive)
				.get<model>(archive)
				.get<shader>(archive);
		}

		// -- Save light information --
		nlohmann::json& lighting_json = project_json["lighting"];
		nlohmann::json& directional_light_json = lighting_json["directional"];
		nlohmann::json& point_light_json = lighting_json["point"];

		// Directional lighting
		directional_light_json["direction"]	= vec3_to_json(shader::directional_direction);
		directional_light_json["ambient"]	= vec3_to_json(shader::directional_ambient);
		directional_light_json["diffuse"]	= vec3_to_json(shader::directional_diffuse);
		directional_light_json["specular"]	= vec3_to_json(shader::directional_specular);

		// Point lights
		nlohmann::json point_lights;
		for (u16 i = 0; i < shader::point_light_count; ++i)
		{
			nlohmann::json point_light;
			point_light["name"] = shader::point_lights[i].name;

			point_light["ambient"] = vec3_to_json(shader::point_lights[i].ambient);
			point_light["diffuse"] = vec3_to_json(shader::point_lights[i].diffuse);
			point_light["specular"] = vec3_to_json(shader::point_lights[i].specular);

			point_light["position"] = vec3_to_json(shader::point_lights[i].position);

			nlohmann::json& attenuation = point_light["attenuation"];
			attenuation["constant"] = shader::point_lights[i].attenuation_constant;
			attenuation["linear"] = shader::point_lights[i].attenuation_linear;
			attenuation["quadratic"] = shader::point_lights[i].attenuation_quadratic;

			point_lights.push_back(point_light);
		}
		point_light_json = point_lights;

		io::write_json_file(file_path, project_json);
	}

	nlohmann::json project::default_project()
	{
		nlohmann::json json;
		json[keys::version] = version;
		json[keys::scene_name_list] = { default_main_scene_name };

		return json;
	}

	std::string project::path() const
	{
		return file_path;
	}

	void project::save_camera_settings(const camera& camera)
	{
		nlohmann::json& camera_json = project_json["camera"];

		camera_json["position"] 			= { camera.position.x, camera.position.y, camera.position.z };
		camera_json["yaw"]					= camera.yaw;
		camera_json["pitch"]				= camera.pitch;
		camera_json["movement_speed"]		= camera.movement_speed;
		camera_json["mouse_sensitivity"]	= camera.mouse_sensitivity;
		camera_json["fov"]					= camera.fov;
		camera_json["editor_mode"]			= camera.editor_mode;
	}

	void project::load_camera_settings(camera& camera)
	{
		// Load camera settings if they can be found from the project data
		if (project_json.contains("camera"))
		{
			nlohmann::json& camera_json = project_json["camera"];

			camera.position				= json_to_vec3<f32>(camera_json["position"]).to_glm_vec();
			camera.yaw					= camera_json["yaw"];
			camera.pitch				= camera_json["pitch"];
			camera.movement_speed		= camera_json["movement_speed"];
			camera.mouse_sensitivity	= camera_json["mouse_sensitivity"];
			camera.fov					= camera_json["fov"];
			camera.editor_mode			= camera_json["editor_mode"];
		}
	}

	void project::load_project()
	{
		ensure(birb::opengl_initialized, "OpenGL needs to have been initialized before projects can be loaded");
		ensure(project_loaded == false);
		project_loaded = true;

		birb::log("Loading project: " + file_path);

		// Load all scenes
		std::vector<std::string> scene_names = project_json[keys::scene_name_list];

		for (const std::string& name : scene_names)
		{
			birb::log("Processing scene " + name);

			scene& scene = scene_collection[name];
			scene.name = name;

			loaded_version = project_json[keys::version];

			// Load entity data if the entity data file exists
			const std::string& entity_data_path = name + entity_data_file_extension;

			if (std::filesystem::exists(entity_data_path))
			{
				std::ifstream entity_archive(entity_data_path, std::ios::binary);
				cereal::BinaryInputArchive archive(entity_archive);

				switch (loaded_version)
				{
					case (1):
					{
						entt::snapshot_loader{scene.registry}
							.get<entt::entity>(archive)
							.get<info>(archive)
							.get<transform>(archive)
							.get<material>(archive)
							.get<model>(archive)
							.get<shader>(archive);
						break;
					}


					default:
						birb::log_fatal(1, "Unsupported project version: ", version);
						break;
				}
			}

			// Load lighting information
			nlohmann::json& lighting_json = project_json["lighting"];
			nlohmann::json& directional_light_json = lighting_json["directional"];
			nlohmann::json& point_light_json = lighting_json["point"];

			shader::directional_direction	= json_to_vec3<f32>(directional_light_json["direction"]);
			shader::directional_ambient		= json_to_vec3<f32>(directional_light_json["ambient"]);
			shader::directional_diffuse		= json_to_vec3<f32>(directional_light_json["diffuse"]);
			shader::directional_specular	= json_to_vec3<f32>(directional_light_json["specular"]);

			for (u16 i = 0; i < shader::point_light_count; ++i)
			{
				nlohmann::json& point_light = point_light_json[i];

				shader::point_lights[i].name = point_light["name"];

				shader::point_lights[i].ambient = json_to_vec3<f32>(point_light["ambient"]);
				shader::point_lights[i].diffuse = json_to_vec3<f32>(point_light["diffuse"]);
				shader::point_lights[i].specular = json_to_vec3<f32>(point_light["specular"]);

				shader::point_lights[i].position = json_to_vec3<f32>(point_light["position"]);

				const nlohmann::json& attenuation = point_light["attenuation"];
				shader::point_lights[i].attenuation_constant = attenuation["constant"];
				shader::point_lights[i].attenuation_linear = attenuation["linear"];
				shader::point_lights[i].attenuation_quadratic = attenuation["quadratic"];
			}


			// Compile shaders
			const auto shader_view = scene.registry.view<shader>();
			for (const auto entity : shader_view)
			{
				shader_view.get<birb::shader>(entity).compile();
			}

			// Load all 3D models
			const auto model_view = scene.registry.view<model>();
			for (const auto entity : model_view)
			{
				birb::model& model = model_view.get<birb::model>(entity);
				model.load_model();
			}
		}
	}

	void project::write_empty_project_to_file(const std::string& dest_path)
	{
		ensure(!dest_path.empty());

		nlohmann::json json = default_project();

		// Write the json data to file
		io::write_json_file(dest_path, json);
	}
}
