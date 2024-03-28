#pragma once

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
		constexpr unsigned short toggle_wireframe_rendering_mode = 1;

		/**
		 * @brief Set window background clear color
		 *
		 * data: float[3]
		 */
		constexpr unsigned short set_window_background_clear_color = 2;

		/**
		 * @brief Reserved for testing purposes
		 */
		constexpr unsigned short reserved_test_event = 999;
	}
}
