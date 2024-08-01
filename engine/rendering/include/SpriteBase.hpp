#pragma once

#include "AspectRatioLock.hpp"

namespace birb
{
	struct sprite_base
	{
		bool ignore_aspect_ratio = false;
		bool orthographic_projection = true;

		birb::aspect_ratio_lock aspect_ratio_lock = aspect_ratio_lock::height;
	};
}
