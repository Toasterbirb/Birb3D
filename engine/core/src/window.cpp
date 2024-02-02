#include "Window.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <queue>
#include <string>

namespace birb
{
	static std::queue<input> input_queue;

	window::window(const std::string& title, const vec2<unsigned int> dimensions)
	:dimensions(dimensions)
	{
		assert(!title.empty() && "Empty window title");

		birb::log("Spawning a new window: " + title + " " + dimensions.to_string());

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

		// Assign the key callback function
		glfwSetKeyCallback(this->glfw_window, key_callback);

		birb::log("window created successfully!");
	}

	window::~window()
	{
		birb::log("Destroying the window");
		glfwDestroyWindow(this->glfw_window);

		birb::log("Terminating GLFW");
		glfwTerminate();
	}

	bool window::should_close() const
	{
		return glfwWindowShouldClose(this->glfw_window) || force_should_quit;
	}

	void window::quit()
	{
		this->force_should_quit = true;
	}

	void window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void window::flip()
	{
		glfwSwapBuffers(this->glfw_window);
	}

	void window::poll()
	{
		glfwPollEvents();
	}

	bool window::inputs_available() const
	{
		return !input_queue.empty();
	}

	input window::next_input()
	{
		assert(!input_queue.empty() && "Tried to read inputs when there were none. Remember to check inputs_available() before calling this function.");

		input top_input = input_queue.front();
		input_queue.pop();
		return top_input;
	}

	void window::forget_inputs()
	{
		std::queue<input> empty_queue;
		std::swap(input_queue, empty_queue);
	}


	void window::key_callback(__attribute_maybe_unused__ GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		input new_input {
			.scancode	= scancode,
			.mods		= mods,
			.key		= static_cast<input::keycode>(key),
			.state		= static_cast<input::action>(action),
		};

		input_queue.push(new_input);
	}
}
