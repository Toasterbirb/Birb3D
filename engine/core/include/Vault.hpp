#pragma once

#include "Assert.hpp"
#include "Vector.hpp"

#include <concepts>
#include <nlohmann/json.hpp>

namespace birb
{
	class vault
	{
	public:
		vault(const std::string& application_name);
		~vault();

		/**
		 * @brief Remove all values from the vault
		 */
		void clear();

		/**
		 * @brief Save the vault to disk
		 *
		 * @param obfuscate Obfuscate the save data
		 */
		void save(const bool obfuscate = false);

		/**
		 * @brief Check if the vault is newly created
		 *
		 * This might mean that some values might not yet
		 * exist in the save file and thus need to be
		 * initialized
		 */
		bool is_new() const;

		/**
		 * @brief Check if the vault contains a key
		 */
		bool contains(const std::string& key, const std::string& category = "default") const;

		template<typename T>
		__attribute__((always_inline))
		void set(const T value, const std::string& key, const std::string& category = "default")
		{
			static_assert(std::is_trivial_v<T>, "The default set() method shouldn't be used for non-trivial types");
			json[category][key] = value;
		}

		template<typename T>
		__attribute__((always_inline))
		void set(const std::vector<T>& value, const std::string& key, const std::string& category = "default")
		{
			static_assert(std::is_trivial_v<T>, "The vector set() method shouldn't be used for vectors of non-trivial types");
			json[category][key] = value;
		}

		template<typename T>
		__attribute__((always_inline))
		void set_vec2(const vec2<T> value, const std::string& key, const std::string& category = "default")
		{
			json[category][key]["x"] = value.x;
			json[category][key]["y"] = value.y;
		}

		template<typename T>
		__attribute__((always_inline))
		void set_vec3(const vec3<T> value, const std::string& key, const std::string& category = "default")
		{
			json[category][key]["x"] = value.x;
			json[category][key]["y"] = value.y;
			json[category][key]["z"] = value.z;
		}

		/**
		 * @brief Get a value from the vault
		 */
		template<typename T>
		T get(const std::string& key, const std::string& category = "default") const
		{
			ensure(contains(key, category), "The key doesn't exist in the vault! Use the fallback version of get() if the existence of the value is not guaranteed");
			return json.at(category).at(key);
		}

		template<typename T>
		vec2<T> get_vec2(const std::string& key, const std::string& category = "default") const
		{
			ensure(contains(key, category), "The key doesn't exist in the vault! Use the fallback version of get() if the existence of the value is not guaranteed");
			ensure(json.at(category).at(key).contains("x"), "Malformed vec2");
			ensure(json.at(category).at(key).contains("y"), "Malformed vec2");

			return vec2<T>(
					json.at(category).at(key).at("x"),
					json.at(category).at(key).at("y")
				);
		}

		template<typename T>
		vec3<T> get_vec3(const std::string& key, const std::string& category = "default") const
		{
			ensure(contains(key, category), "The key doesn't exist in the vault! Use the fallback version of get() if the existence of the value is not guaranteed");
			ensure(json.at(category).at(key).contains("x"), "Malformed vec3");
			ensure(json.at(category).at(key).contains("y"), "Malformed vec3");
			ensure(json.at(category).at(key).contains("z"), "Malformed vec3");

			return vec3<T>(
					json.at(category).at(key).at("x"),
					json.at(category).at(key).at("y"),
					json.at(category).at(key).at("z")
				);
		}

		/**
		 * @brief Get a value from the vault
		 *
		 * If the key doesn't exist in the vault, return
		 * the fallback value
		 */
		template<typename T>
		T get(const T fallback_value, const std::string& key, const std::string& category = "default") const
		{
			if (contains(key, category))
				return json.at(category).at(key);
			else
				return fallback_value;
		}

	private:
		nlohmann::json json;
		std::string file_path;
		bool _is_new = true;
	};
}
