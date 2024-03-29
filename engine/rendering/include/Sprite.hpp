#pragma once

#include "Texture.hpp"

#include <memory>
#include <string>

namespace birb
{
	struct sprite
	{
		explicit sprite(const std::string& file_path, color_format format = color_format::RGBA);
		~sprite() = default;
		sprite(const sprite& other);
		sprite(sprite& other);

		std::shared_ptr<birb::texture> texture;
	};
}
