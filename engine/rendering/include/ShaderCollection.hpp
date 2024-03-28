#pragma once

#include "Shader.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace birb
{
	class shader_collection
	{
	public:
		~shader_collection();

		/**
		 * @brief Get a reference to a shader built from the given shader source names
		 *
		 * If the shader hasn't been compiled yet when its requested for,
		 * it'll get compiled once and after that it can be re-used
		 */
		static std::shared_ptr<shader> get_shader(const std::string& vertex, const std::string& fragment);

		/**
		 * @brief Clear the shader collection
		 */
		static void wipe();

	private:
		static inline std::unordered_map<std::string, std::shared_ptr<shader>> shader_storage;
	};
}
