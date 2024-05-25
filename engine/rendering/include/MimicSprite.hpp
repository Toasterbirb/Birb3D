#pragma once

#include "AspectRatioLock.hpp"

namespace birb
{
	class texture;

	struct mimic_sprite
	{
		mimic_sprite(birb::texture* texture);
		~mimic_sprite() = default;
		mimic_sprite(const mimic_sprite&) = default;
		mimic_sprite(mimic_sprite&) = default;

		birb::texture* texture;

		bool ignore_aspect_ratio = false;
		birb::aspect_ratio_lock aspect_ratio_lock = aspect_ratio_lock::height;
	};
}
