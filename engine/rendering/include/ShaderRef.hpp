#pragma once

#include "Types.hpp"

#include <string>

namespace birb
{
	struct shader_ref
	{
		shader_ref(const std::string& shader_name);
		shader_ref(const std::string& vertex, const std::string& fragment);

		/**
		 * @brief A hash generated from the vertex and fragment shader names
		 */
		u64 hash = 0;

		/**
		 * @brief A hash for identify the vertex shader
		 */
		u64 vertex;

		/**
		 * @brief A hash for identifying the fragment
		 */
		u64 fragment;

		/**
		 * @brief Recalculate hashes with new vertex and fragment shader names
		 *
		 * @param vertex	Name of the vertex shader
		 * @param fragment 	Name of the fragment shader
		 */
		void update_hashes(const std::string& vertex, const std::string& fragment);
	};
}
