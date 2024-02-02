#include "Window.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <queue>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace birb
{
	// Input queue for the game/application
	static std::queue<input> input_queue;

	// Input queue for engine reserved key inputs
	static std::queue<input> engine_input_queue;

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

		// Disable vsync
		glfwSwapInterval(0);

		birb::log("window created successfully!");
	}

	window::~window()
	{
		// If ImGui was initialize, quit it gracefully
		if (this->imgui_initialized)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

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
		// Draw the performance widget
		if (this->perf_widget_visible)
			this->perf_widget.draw();

		/* If ImGui has been initialized, let it draw its
		 * stuff before swapping the buffers */
		if (this->imgui_initialized)
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(this->glfw_window);
	}

	void window::poll()
	{
		glfwPollEvents();

		// Process engine inputs
		while (!engine_input_queue.empty())
		{
			// Key down events
			if (engine_input_queue.front().state == input::action::KEY_DOWN)
			{
				switch(engine_input_queue.front().key)
				{
					case (input::keycode::F1):
						this->perf_widget_visible = !this->perf_widget_visible;
						break;

					default:
						break;
				}
			}

			engine_input_queue.pop();
		}

		// If ImGui was initialized, start a new frame
		if (this->imgui_initialized)
			new_imgui_frame();
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

	void window::init_imgui()
	{
		birb::log("Initializing ImGui");
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup renderer backend
		ImGui_ImplGlfw_InitForOpenGL(this->glfw_window, true);
		ImGui_ImplOpenGL3_Init();

		imgui_initialized = true;

		// Start the first ImGui frame
		new_imgui_frame();
	}


	void window::new_imgui_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void window::key_callback(__attribute_maybe_unused__ GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		input new_input {
			.scancode	= scancode,
			.mods		= mods,
			.key		= static_cast<input::keycode>(key),
			.state		= static_cast<input::action>(action),
		};

		// Handle reserved keybindings
		switch (new_input.key)
		{
			case(input::keycode::F1):
				engine_input_queue.push(new_input);
				break;

			default:
				input_queue.push(new_input);
				break;
		}
	}
}
