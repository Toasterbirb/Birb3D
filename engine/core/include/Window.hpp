#pragma once
#define GLFW_INCLUDE_NONE

#ifdef BIRB_PLATFORM_WINDOWS
#ifndef GLFW_DLL
#define GLFW_DLL
#endif
#endif

#include "Color.hpp"
#include "EventBus.hpp"
#include "Input.hpp"
#include "Vector.hpp"

#include <string>
#include <unordered_set>

struct GLFWwindow;

namespace birb
{
	class window : public event_obj
	{
	public:
		/**
		 * @class opt
		 * @brief Window options
		 *
		 */
		struct opt
		{
			constexpr static inline u32 msaa_0					= 1 << 0;
			constexpr static inline u32 msaa_2					= 1 << 1;
			constexpr static inline u32 msaa_4					= 1 << 2;
			constexpr static inline u32 msaa_8					= 1 << 3;
			constexpr static inline u32 resizable				= 1 << 4;
			constexpr static inline u32 transparent 			= 1 << 5;
			constexpr static inline u32 vsync					= 1 << 6;
			constexpr static inline u32 lock_cursor_to_window	= 1 << 7;
		};

		/**
		 * @brief Create a new window and initialize graphics related things
		 *
		 * @param title Window title
		 * @param dimensions Size of the window
		 */
		window(const std::string& title, const vec2<i32> dimensions, const u32 window_options = 0);
		~window();
		window(window&) = delete;
		window(const window&) = delete;
		window(window&&) = default;

		void process_event(u16 event_id, const event_data& data) override;

		/**
		 * @brief Check if the window should be still kept open
		 *
		 * @return True if the window has received a quit event etc.
		 */
		bool should_close() const;

		/**
		 * @return The current dimensions of the window
		 */
		vec2<i32> size() const
		{
			return dimensions;
		}

		/**
		 * @brief Clear the window back buffer
		 *
		 * Call this function before starting to render new stuff. Otherwise
		 * the previous frame will be "burnt in" and the next frame will be
		 * the combination of the new and previous frames
		 */
		void clear();

		/**
		 * @brief Swap the front and back buffers
		 *
		 * Call this function when you are done rendering everything and want
		 * to present the new frame
		 */
		void flip();

		/**
		 * @brief Poll for and process events
		 *
		 * Any keyboard inputs that have happened between the previous call this this
		 * function and now will be stored into a queue. These inputs can be retrieved
		 * with next_input()
		 */
		void poll();

		/**
		 * @brief Check if there are inputs in the queue ready to be processed
		 *
		 * @return True if there are inputs in the queue, false if the queue is empty
		 */
		bool inputs_available() const;

		/**
		 * @brief Force the \c should_close() function to return true
		 *
		 * @note This won't close the window immediately
		 */
		void quit();

		/**
		 * @brief Get the next keyboard input in the queue and pop it from the input queue
		 *
		 * @return \ref birb::input that contains information about the first keyboard input in the queue
		 */
		input next_input();

		/**
		 * @brief Check if a key is being held down
		 */
		bool is_key_held(birb::input::keycode key) const;

		/**
		 * @brief Clear the keyboard input queue
		 *
		 * This is useful in situations where you haven't had the chance to handle
		 * keyboard inputs for some time. If you don't forget the old inputs in this case,
		 * you'd have to handle all of the accumulated inputs all at once and those inputs
		 * may or may not be even relevant at that point anymore.
		 */
		void forget_inputs();

		/**
		 * @brief Get the current mouse cursor position
		 */
		vec2<f64> cursor_pos();

		/**
		 * @brief Hide the cursor and lock it into the window
		 */
		void lock_cursor_to_window();

		/**
		 * @brief Show the cursor and unlock it from the window
		 */
		void unlock_cursor_from_window();

		/**
		 * @brief Check if the cursor is locked to the window
		 */
		static bool is_cursor_locked_to_window();

		/**
		 * @brief Initialize the ImGui library
		 *
		 * This needs to be called before using anything ImGui related
		 */
		void init_imgui();

		/**
		 * @return The refreshrate of the primary monitor
		 */
		i32 monitor_refreshrate() const;

		/**
		 * @brief Set the default background color
		 */
		void set_background_color(const birb::color& color);

		/**
		 * @return The currently set background clear color of the window
		 */
		color background_color() const;

		/**
		 * @brief Automatically resize the viewport when window gets resized
		 */
		bool viewport_autoresize = true;

		/**
		 * @brief MSAA sample count
		 */
		u8 msaa_level() const;

		static inline bool hot_reload_assets_on_focus_change = false;

	private:
		GLFWwindow* glfw_window;
		color current_background_color;
		u8 _msaa_level{};

		/**
		 * @brief Current size of the window
		 *
		 * @note This variable shouldn't be modified manually during runtime.
		 * It will be kept up-to-date in the poll() function with the help
		 * of window_size_callback()
		 */
		vec2<i32> dimensions;

		static inline bool cursor_locked_to_window = false;

		/**
		 * @brief Override for the should_close() function. Can be set to true with quit()
		 */
		bool force_should_quit = false;

		void new_imgui_frame();

		// Static variables for callback functions
		static inline bool window_size_changed;
		static inline std::unordered_set<birb::input::keycode> held_down_keys;
		static void update_held_down_keys(const birb::input& input);

		/**
		 * @brief GLFW input callback function
		 */
		static void key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);

		/**
		 * @brief GLFW mouse input callback function
		 */
		static void mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods);

		/**
		 * @brief GLFW mouse scrolling callback function
		 */
		static void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset);

		/**
		 * @brief GLFW error callback function
		 */
		static void error_callback(i32 error, const char* description);

		/**
		 * @brief GLFW window resize callback function
		 */
		static void window_size_callback(GLFWwindow* window, i32 width, i32 height);

		/**
		 * @brief GLFW window focus callback function
		 */
		static void window_focus_callback(GLFWwindow* window, int focused);
	};
}
