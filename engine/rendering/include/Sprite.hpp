#pragma once

#include "AspectRatioLock.hpp"
#include "ColorFormat.hpp"

#include <memory>
#include <string>

namespace birb
{
	class texture;

	struct sprite
	{
		explicit sprite(const std::string& file_path, color_format format = color_format::RGBA);
		~sprite() = default;
		sprite(const sprite& other) = default;
		sprite(sprite& other) = default;

		std::shared_ptr<birb::texture> texture;
		bool ignore_aspect_ratio = false;
		bool orthographic_projection = true;

		birb::aspect_ratio_lock aspect_ratio_lock = aspect_ratio_lock::height;
	};
}
