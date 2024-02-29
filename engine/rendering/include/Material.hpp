#pragma once

#include "Texture.hpp"

namespace birb
{
	class material
	{
	public:
		material(const std::string& diffuse_map, const std::string& specular_map, float shininess);

		void apply_to_shader(shader& shader);

		float shininess;

	private:
		texture diffuse, specular;
	};
}
