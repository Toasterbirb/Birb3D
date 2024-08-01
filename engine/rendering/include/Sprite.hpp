#pragma once

#include "AspectRatioLock.hpp"
#include "Color.hpp"
#include "ColorFormat.hpp"
#include "SpriteBase.hpp"

#include <memory>
#include <string>

namespace birb
{
	class texture;

	struct sprite : public sprite_base
	{
		explicit sprite(const std::string& file_path, color_format format = color_format::RGBA);
		~sprite() = default;
		sprite(const sprite& other) = default;
		sprite(sprite& other) = default;

		std::shared_ptr<birb::texture> texture;
		birb::color color;
	};
}
