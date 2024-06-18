#pragma once

#include "Assert.hpp"

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
		 */
		void save();

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
			json[category][key] = value;
		}

		template<typename T>
		__attribute__((always_inline))
		void set(const std::vector<T>& value, const std::string& key, const std::string& category = "default")
		{
			json[category][key] = value;
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
