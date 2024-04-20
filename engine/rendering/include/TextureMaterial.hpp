#pragma once

#include "Texture.hpp"

namespace birb
{
	class texture_material
	{
	public:
		texture_material(const std::string& diffuse_map, const std::string& specular_map, f32 shininess);

		void apply_to_shader(shader& shader);

		f32 shininess;

	private:
		texture diffuse, specular;
	};
}
