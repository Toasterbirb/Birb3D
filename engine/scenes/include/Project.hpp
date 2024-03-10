#pragma once

#include "Camera.hpp"
#include "Scene.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace birb
{
	class project
	{
	public:
		project();
		explicit project(const std::string& path);
		~project() = default;
		project(const project&) = default;
		project(project&) = default;

		void save();
		void load(const std::string& path);
		static void write_empty_project_to_file(const std::string& dest_path);
		static nlohmann::json default_project();
		std::string path() const;

		// Camera stuff
		void save_camera_settings(const camera& camera);
		void load_camera_settings(camera& camera);

		std::unordered_map<std::string, scene> scene_collection;


		/**
		 * @brief The version of the project format
		 *
		 * Increment this whenever there are breaking changes
		 * so that they can be handled properly when loading the project
		 */
		static inline unsigned int version = 1;

	private:
		bool project_loaded = false;

		void load_project();
		nlohmann::json project_json;
		std::string file_path;

		/**
		 * @brief The name of the first scene in a new project
		 */
		const static inline std::string default_main_scene_name = "main";

		const static inline std::string entity_data_file_extension = ".scn";

		// Helper functions
		template<typename T>
		vec3<T> json_to_vec3(nlohmann::json json)
		{
			return vec3<T>(json[0], json[1], json[2]);
		}

		template<typename T>
		nlohmann::json vec3_to_json(vec3<T> vector)
		{
			nlohmann::json json;
			json = { vector.x, vector.y, vector.z };
			return json;
		}
	};
}
