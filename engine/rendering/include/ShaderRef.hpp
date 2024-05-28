#pragma once

#include "Assert.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include <string>

namespace birb
{
	struct shader_ref
	{
		shader_ref(const std::string& vertex, const std::string& fragment)
		{
			ensure(!vertex.empty());
			ensure(!fragment.empty());

			this->vertex = std::hash<std::string>{}(vertex);
			this->fragment = std::hash<std::string>{}(fragment);

			// Some hash combination thing found from cppreference
			hash = combine_hashes(this->vertex, this->fragment);
		}


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
	};
}
