#pragma once

#include <cassert>
#include <string>

namespace birb
{
	struct shader_ref
	{
		shader_ref(const std::string& vertex, const std::string& fragment)
		:vertex(vertex), fragment(fragment)
		{
			assert(!vertex.empty());
			assert(!fragment.empty());
		}

		/**
		 * @brief Name of the vertex shader
		 */
		std::string vertex;

		/**
		 * @brief Name of the fragment shader
		 */
		std::string fragment;
	};
}
