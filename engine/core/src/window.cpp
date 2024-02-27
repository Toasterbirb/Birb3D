#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "EventBus.hpp"
#include "Profiling.hpp"
#include "Window.hpp"

#include <algorithm>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <queue>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace birb
{
	// How many windows have been created so far. This should NEVER be more than 1
	static unsigned short window_count = 0;

	// Input queue for the game/application
	static std::queue<input> input_queue;

	// Input queue for engine reserved key inputs
	static std::queue<input> engine_input_queue;

	window::window(const std::string& title, const vec2<int> dimensions)
	:dimensions(dimensions)
	{
		if (window_count != 0)
			birb::log_fatal("There can only be one window at any given time");

		assert(!title.empty() && "Empty window title");
		assert(dimensions.x > 0 && "Invalid window width");
		assert(dimensions.y > 0 && "Invalid window height");

		window_count++;

		birb::log("Spawning a new window: " + title + " " + dimensions.to_string());

		// Initialize the glfw library
		birb::log("Initializing GLFW");
		if (!glfwInit())
			birb::log_fatal("Can't initialize GLFW");

		// Set some hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create the window
		birb::log("Creating the window");
		glfw_window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), NULL, NULL);
		if (!glfw_window)
		{
			glfwTerminate();
			birb::log_fatal("Can't create a new GLFW window");
		}

		// Set GLFW callback functions
		glfwMakeContextCurrent(glfw_window);
		glfwSetKeyCallback(glfw_window, key_callback);
		glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
		glfwSetErrorCallback(error_callback);
		glfwSetWindowSizeCallback(glfw_window, window_size_callback);

		// Disable vsync
		glfwSwapInterval(0);

		// Setup glad
		gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		glViewport(0, 0, dimensions.x, dimensions.y);

		// Set static variables
		window::window_size_changed = false;
		window::imgui_initialized = false;

		// Set the default background color to something other than black
		set_background_color(0x1F1F28);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);


		birb::log("window created successfully!");
	}

	window::~window()
	{
		// If ImGui was initialize, quit it gracefully
		if (window::imgui_initialized)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		birb::log("Destroying the window");
		glfwDestroyWindow(glfw_window);

		birb::log("Terminating GLFW");
		glfwTerminate();
	}

	bool window::should_close() const
	{
		return glfwWindowShouldClose(glfw_window) || force_should_quit;
	}

	void window::quit()
	{
		force_should_quit = true;
	}

	void window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void window::flip()
	{
		PROFILER_SCOPE_RENDER_FN()
		/* If ImGui has been initialized, let it draw its
		 * stuff before swapping the buffers */
		if (window::imgui_initialized)
		{
			PROFILER_SCOPE_RENDER("ImGui render");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(glfw_window);
		MicroProfileFlip(nullptr);

		// If ImGui was initialized, start a new frame
		if (window::imgui_initialized)
			new_imgui_frame();
	}

	void window::poll()
	{
		PROFILER_SCOPE_INPUT_FN()
		glfwPollEvents();

		// Update window dimensions and viewport size if needed
		if (window::window_size_changed && viewport_autoresize)
		{
			window::window_size_changed = false;
			glfwGetWindowSize(glfw_window, &dimensions.x, &dimensions.y);

			assert(dimensions.x != 0 && "Chance for a zero division");
			assert(dimensions.y != 0 && "Chance for a zero division");

			glViewport(0, 0, dimensions.x, dimensions.y);
		}

		// Process engine inputs
		while (!engine_input_queue.empty())
		{
			// Key down events
			if (engine_input_queue.front().state == input::action::KEY_DOWN)
			{
				switch(engine_input_queue.front().key)
				{
					case birb::input::keycode::F1:
						event_bus::send_event(1);
						break;

					default:
						break;
				}
			}

			engine_input_queue.pop();
		}
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

	bool window::is_key_held(birb::input::keycode key) const
	{
		return held_down_keys.contains(key);
	}

	void window::forget_inputs()
	{
		std::queue<input> empty_queue;
		std::swap(input_queue, empty_queue);
	}

	vec2<double> window::cursor_pos()
	{
		vec2<double> pos;

		glfwGetCursorPos(glfw_window, &pos.x, &pos.y);

		if (!cursor_locked_to_window)
		{
			// Clamp mouse coords to window size
			pos.x = std::clamp(pos.x, 0.0, static_cast<double>(dimensions.x));
			pos.y = std::clamp(pos.y, 0.0, static_cast<double>(dimensions.y));
		}

		return pos;
	}

	void window::lock_cursor_to_window()
	{
		if (!cursor_locked_to_window)
		{
			glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursor_locked_to_window = true;
		}
	}

	void window::unlock_cursor_from_window()
	{
		if (cursor_locked_to_window)
		{
			glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursor_locked_to_window = false;
		}
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
		ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
		ImGui_ImplOpenGL3_Init();

		window::imgui_initialized = true;

		// Change some colors following the kanagawa.nvim colorscheme
		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(0.862745f, 0.843137f, 0.729412f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0862745f, 0.0862745f, 0.113725f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.211765f, 0.211765f, 0.27451f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.176471f, 0.309804f, 0.403922f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.862745f, 0.843137f, 0.729412f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.596078f, 0.733333f, 0.423529f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.462745f, 0.580392f, 0.415686f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.164706f, 0.164706f, 0.215686f, 0.7f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.145098f, 0.145098f, 0.207843f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.329412f, 0.329412f, 0.427451f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.176471f, 0.309804f, 0.403922f, 1.0f);

		// Start the first ImGui frame
		new_imgui_frame();
	}

	bool window::imgui_is_init()
	{
		return window::imgui_initialized;
	}

	int window::monitor_refreshrate() const
	{
		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return vid_mode->refreshRate;
	}

	void window::set_background_color(birb::color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


	void window::new_imgui_frame()
	{
		PROFILER_SCOPE_RENDER_FN()

		{
			PROFILER_SCOPE_RENDER("ImGui_ImplOpenGL3_NewFrame")
			ImGui_ImplOpenGL3_NewFrame();
		}

		{
			PROFILER_SCOPE_RENDER("ImGui_ImplGlfw_NewFrame")
			ImGui_ImplGlfw_NewFrame();
		}

		{
			PROFILER_SCOPE_RENDER("ImGui::NewFrame")
			ImGui::NewFrame();
		}
	}

	void window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		input new_input {
			.scancode	= scancode,
			.mods		= mods,
			.key		= static_cast<input::keycode>(key),
			.state		= static_cast<input::action>(action),
			.pos		= {-1, -1},
		};

		// Keep the "held down keys" list up-to-date
		if (new_input.state == birb::input::action::KEY_DOWN)
		{
			held_down_keys.insert(new_input.key);
		}
		else if (new_input.state == birb::input::action::KEY_UP)
		{
			assert(held_down_keys.contains(new_input.key) && "Tried to remove a key from held down keys when it wasn't in the set in the first place");
			held_down_keys.erase(new_input.key);
		}

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

	void window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		input new_input {
			.scancode	= 0,
			.mods		= mods,
			.key		= static_cast<input::keycode>(button),
			.state		= static_cast<input::action>(action),
			.pos		= vec2<double>(0,0),
		};

		// Get the current mouse position
		glfwGetCursorPos(window, &new_input.pos.x, &new_input.pos.y);

		input_queue.push(new_input);
	}

	void window::error_callback(int error, const char* description)
	{
		birb::log_error("GLFW error [" + std::to_string(error) + "]: " + std::string(description));
	}

	void window::window_size_callback(GLFWwindow* window, int width, int height)
	{
		window::window_size_changed = true;
	}
}
