#pragma once

#include "AspectRatioLock.hpp"

namespace birb
{
	struct sprite_base
	{
		bool ignore_aspect_ratio = false;
		bool orthographic_projection = true;

		birb::aspect_ratio_lock aspect_ratio_lock = aspect_ratio_lock::height;

		/**
		 * @brief Editor UI for the sprite base that all sprites should draw
		 */
		void draw_editor_base_ui();
	};
}
