#pragma once

#include "Texture.hpp"

namespace birb
{
	class texture_material
	{
	public:
		texture_material(const std::string& diffuse_map, const std::string& specular_map, float shininess);

		void apply_to_shader(shader& shader);

		float shininess;

	private:
		texture diffuse, specular;
	};
}
