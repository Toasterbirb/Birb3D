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
		 * data: f32[3]
		 */
		constexpr u16 set_window_background_clear_color = 2;

		/**
		 * @brief Toggle debug widgets in the renderer
		 *
		 * data: none
		 */
		constexpr u16 toggle_debug_widgets = 3;

		/**
		 * @brief Toggle debug overlays in the renderer
		 *
		 * data: none
		 */
		constexpr u16 toggle_debug_overlays = 4;

		/**
		 * @brief Update cached projection matrices in cameras
		 *
		 * data: vec2<i32>
		 */
		constexpr u16 window_resized = 5;

		/**
		 * @brief Hot reload all models in the currently active scene
		 *
		 * data: none
		 */
		constexpr u16 reload_models = 10;

		/**
		 * @brief Reserved for testing purposes
		 */
		constexpr u16 reserved_test_event = 999;
	}
}
