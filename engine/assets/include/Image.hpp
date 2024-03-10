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

			unsigned char* data;
			vec2<int> dimensions;
			int color_channels;
		};
	}
}
