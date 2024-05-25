#include "Assert.hpp"
#include "MimicSprite.hpp"
#include "Texture.hpp"

namespace birb
{
	mimic_sprite::mimic_sprite(birb::texture* texture)
	{
		ensure(texture != nullptr);
	}
}
