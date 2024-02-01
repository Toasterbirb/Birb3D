#pragma once

#include "Vector.hpp"

#include <string>
#include <GLFW/glfw3.h>

namespace birb
{
	class Window
	{
	public:
		// Create a new window
		Window(const std::string& title, const vec2<unsigned int> dimensions);
		~Window();

		// Check if the window should be still kept open
		bool should_close() const;

		// Clear the window buffer
		void clear();

		// Swap front and back buffers
		void flip();

		// Poll for and process events
		void poll();

	private:
		GLFWwindow* glfw_window;
		vec2<unsigned int> dimensions;
	};
}
