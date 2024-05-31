#pragma once

#include "Types.hpp"

#include <string>

namespace birb
{
	class shader_ref
	{
	public:
		shader_ref(const std::string& shader_name);
		shader_ref(const std::string& vertex, const std::string& fragment);

		/**
		 * @brief A hash generated from the vertex and fragment shader names
		 */
		u64 hash() const;

		/**
		 * @brief A hash for identify the vertex shader
		 */
		u64 vertex() const;

		/**
		 * @brief A hash for identifying the fragment
		 */
		u64 fragment() const;

		/**
		 * @brief Recalculate hashes with new vertex and fragment shader names
		 *
		 * @param vertex	Name of the vertex shader
		 * @param fragment 	Name of the fragment shader
		 */
		void update_hashes(const std::string& vertex, const std::string& fragment);

	private:
		u64 combined_hash = 0;
		u64 vertex_hash = 0;
		u64 fragment_hash = 0;
	};
}
