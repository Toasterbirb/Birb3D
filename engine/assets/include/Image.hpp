#pragma once

#include "Vector.hpp"

namespace birb
{
	namespace asset
	{
		struct image
		{
			explicit image(const char* path, bool flip_vertically = false);
			~image();
			image(image&) = delete;
			image(const image&) = delete;
			image(image&& other);

			unsigned char* data = nullptr;
			vec2<i32> dimensions;
			i32 color_channels;
		};
	}
}
