#pragma once

#include "Vector.hpp"

namespace birb
{
	/**
	 * @brief True if opengl has been and still is initialized
	 *
	 * This variable gets set during birb::window construction and destruction
	 */
	inline bool g_opengl_initialized = false;

	/**
	 * @brief True if imgui has been and still is initialized
	 *
	 * This variable gets set in birb::window when its method
	 * init_imgui() is called
	 */
	inline bool g_imgui_initialized = false;

	/**
	 * @brief True if the front and back buffers have been flipped and clear() hasn't been called yet
	 *
	 * This variable gets set in birb::window when its methods
	 * flip() and clear() are called
	 */
	inline bool g_buffers_flipped = false;

	/**
	 * @brief Width and height of the window
	 */
	inline vec2<i32> window_dimensions;
}
