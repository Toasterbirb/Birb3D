#include "Window.hpp"

#include <GL/gl.h>
#include <cassert>
#include <string>
#include <GLFW/glfw3.h>

namespace birb
{
	Window::Window(const std::string& title, const vec2<int> dimensions)
	:dimensions(dimensions)
	{
		assert(!title.empty() && "Empty window title");
		assert(dimensions.x > 0 && "Invalid window width");
		assert(dimensions.y > 0 && "Invalid window height");

		birb::log("Spawning a new window: " + title + " [" + std::to_string(dimensions.x) + ", " + std::to_string(dimensions.y) + "]");

		// Initialize the glfw library
		birb::log("Initializing GLFW");
		if (!glfwInit())
			birb::log_fatal("Can't initialize GLFW");


		// Create the window
		birb::log("Creating the window");
		this->glfw_window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), NULL, NULL);
		if (!this->glfw_window)
		{
			glfwTerminate();
			birb::log_fatal("Can't create a new GLFW window");
		}

		// Make the window's context current
		glfwMakeContextCurrent(this->glfw_window);

		birb::log("Window created successfully!");
	}

	Window::~Window()
	{
		birb::log("Destroying the window");
		glfwDestroyWindow(this->glfw_window);

		birb::log("Terminating GLFW");
		glfwTerminate();
	}

	bool Window::should_close() const
	{
		return glfwWindowShouldClose(this->glfw_window);
	}

	void Window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::flip()
	{
		glfwSwapBuffers(this->glfw_window);
	}

	void Window::poll()
	{
		glfwPollEvents();
	}
}
