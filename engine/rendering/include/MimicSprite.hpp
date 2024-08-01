#pragma once

#include "AspectRatioLock.hpp"
#include "SpriteBase.hpp"

namespace birb
{
	class texture;

	struct mimic_sprite : public sprite_base
	{
		mimic_sprite(birb::texture* texture);
		~mimic_sprite() = default;
		mimic_sprite(const mimic_sprite&) = default;
		mimic_sprite(mimic_sprite&) = default;

		birb::texture* texture;
	};
}
