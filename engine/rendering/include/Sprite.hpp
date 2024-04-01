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
		sprite(const sprite& other) = default;
		sprite(sprite& other) = default;

		std::shared_ptr<birb::texture> texture;
		bool ignore_aspect_ratio = false;

		enum class aspect_ratio_lock
		{
			width, height
		};
		aspect_ratio_lock aspect_ratio_lock = sprite::aspect_ratio_lock::height;
	};
}
