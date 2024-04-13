#pragma once

#include <cassert>
#include <memory>
#include <string>

namespace birb
{
	struct shader_ref
	{
		shader_ref(const std::string& vertex, const std::string& fragment)
		{
			assert(!vertex.empty());
			assert(!fragment.empty());

			this->vertex = std::make_shared<std::string>(vertex);
			this->fragment = std::make_shared<std::string>(fragment);
		}

		/**
		 * @brief Name of the vertex shader
		 */
		std::shared_ptr<std::string> vertex;

		/**
		 * @brief Name of the fragment shader
		 */
		std::shared_ptr<std::string> fragment;
	};
}
