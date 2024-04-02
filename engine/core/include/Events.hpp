#pragma once

#include "Types.hpp"

namespace birb
{
	// Engine reserved event IDs
	namespace event
	{

		/**
		 * @brief Toggle wireframe rendering mode in the renderer
		 *
		 * data: none
		 */
		constexpr u16 toggle_wireframe_rendering_mode = 1;

		/**
		 * @brief Set window background clear color
		 *
		 * data: float[3]
		 */
		constexpr u16 set_window_background_clear_color = 2;

		/**
		 * @brief Toggle debug view in the renderer
		 *
		 * data: none
		 */
		constexpr u16 toggle_debug_view = 3;

		/**
		 * @brief Reserved for testing purposes
		 */
		constexpr u16 reserved_test_event = 999;
	}
}
