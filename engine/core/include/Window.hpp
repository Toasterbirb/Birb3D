#pragma once

#include "Input.hpp"
#include "PerformanceWidget.hpp"
#include "Vector.hpp"

#include <string>
#include <GLFW/glfw3.h>

namespace birb
{
	class window
	{
	public:
		// Create a new window
		window(const std::string& title, const vec2<unsigned int> dimensions);
		~window();

		// Check if the window should be still kept open
		bool should_close() const;

		// Clear the window buffer
		void clear();

		// Swap front and back buffers
		void flip();

		// Poll for and process events
		void poll();

		// Check if there are inputs in the queue ready to be processed
		bool inputs_available() const;

		// Force the should_close() function to return true
		// This won't close the window immediately
		void quit();

		// Get the next input in the queue and pop it
		input next_input();

		// Clear the input queue
		void forget_inputs();

		// Initialize ImGui
		void init_imgui();

	private:
		GLFWwindow* glfw_window;
		vec2<unsigned int> dimensions;
		bool force_should_quit = false;

		// This is set to true when init_imgui() has been called
		bool imgui_initialized = false;

		// Start a new ImGui frame
		void new_imgui_frame();

		performance_widget perf_widget;
		bool perf_widget_visible = false;
		void toggle_performance_widget();

		// GLFW input callback function
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}
