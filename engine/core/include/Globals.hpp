#pragma once

namespace birb
{
	/**
	 * @brief True if opengl has been and still is initialized
	 *
	 * This variable gets set during birb::window construction and destruction
	 */
	inline bool opengl_initialized = false;

	/**
	 * @brief True if imgui has been and still is initialized
	 *
	 * This variable gets set in birb::window when its method
	 * init_imgui() is called
	 */
	inline bool imgui_initialized = false;
}
