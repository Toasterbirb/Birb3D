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

		// Initialize the glfw library
		if (!glfwInit())
			return;

		// Create the window
		this->glfw_window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), NULL, NULL);
		if (!this->glfw_window)
		{
			glfwTerminate();
			return;
		}

		// Make the window's context current
		glfwMakeContextCurrent(this->glfw_window);
	}

	Window::~Window()
	{
		glfwDestroyWindow(this->glfw_window);
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
