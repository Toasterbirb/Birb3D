#pragma once

#include "ShaderRef.hpp"
#include "SpriteBase.hpp"

#include <string>

namespace birb
{
	struct shader_sprite : public sprite_base
	{
		explicit shader_sprite(const std::string& fragment_shader_name);
		~shader_sprite() = default;
		shader_sprite(const shader_sprite&) = default;
		shader_sprite(shader_sprite&) = default;

		const shader_ref shader_reference;
	};
}
