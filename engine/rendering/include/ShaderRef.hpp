#pragma once

#include "Assert.hpp"
#include "Types.hpp"

#include <string>

namespace birb
{
	struct shader_ref
	{
		shader_ref(const std::string& vertex, const std::string& fragment)
		{
			ASSERT(!vertex.empty());
			ASSERT(!fragment.empty());

			this->vertex = std::hash<std::string>{}(vertex);
			this->fragment = std::hash<std::string>{}(fragment);

			// Some hash combination thing found from cppreference
			hash = this->vertex ^ (this->fragment << 1);
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
