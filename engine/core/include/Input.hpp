#pragma once
#include "Vector.hpp"

namespace birb
{
	// Container for key input information
	struct input
	{
		enum class action
		{
			key_up = 0,
			key_down = 1,
			key_repeating = 2
		};

		// Keycodes mapped to the GLFW keycodes
		enum class keycode
		{
			mouse_1 = 0,
			mouse_2 = 1,
			mouse_3 = 2,
			mouse_4 = 3,
			mouse_5 = 4,
			mouse_6 = 5,
			mouse_7 = 6,
			mouse_8 = 7,
			mouse_last = 7,
			mouse_left = 0,
			mouse_right = 1,
			mouse_middle = 2,
			space =   32,
			apostrophe =   39, /* ' */
			comma =   44, /* , */
			minus =   45, /* - */
			period =   46, /* . */
			slash =   47, /* / */
			zero = 48,
			one = 49,
			two = 50,
			three = 51,
			four = 52,
			five = 53,
			six = 54,
			seven = 55,
			eight = 56,
			nine = 57,
			semicolon = 59, /* ; */
			equal = 61, /* = */
			a = 65,
			b = 66,
			c = 67,
			d = 68,
			e = 69,
			f = 70,
			g = 71,
			h = 72,
			i = 73,
			j = 74,
			k = 75,
			l = 76,
			m = 77,
			n = 78,
			o = 79,
			p = 80,
			q = 81,
			r = 82,
			s = 83,
			t = 84,
			u = 85,
			v = 86,
			w = 87,
			x = 88,
			y = 89,
			z = 90,
			left_bracket = 91, /* [ */
			backslash = 92, /* \ */
			right_bracket = 93, /* ] */
			grave_accent = 96, /* ` */
			world_1 = 161, /* non-us #1 */
			world_2 = 162, /* non-us #2 */
			escape = 256,
			enter = 257,
			tab = 258,
			backspace = 259,
			insert = 260,
			delete_ = 261,
			right = 262,
			left = 263,
			down = 264,
			up = 265,
			page_up = 266,
			page_down = 267,
			home = 268,
			end = 269,
			caps_lock = 280,
			scroll_lock = 281,
			num_lock = 282,
			print_screen = 283,
			pause = 284,
			f1 = 290,
			f2 = 291,
			f3 = 292,
			f4 = 293,
			f5 = 294,
			f6 = 295,
			f7 = 296,
			f8 = 297,
			f9 = 298,
			f10 = 299,
			f11 = 300,
			f12 = 301,
			f13 = 302,
			f14 = 303,
			f15 = 304,
			f16 = 305,
			f17 = 306,
			f18 = 307,
			f19 = 308,
			f20 = 309,
			f21 = 310,
			f22 = 311,
			f23 = 312,
			f24 = 313,
			f25 = 314,
			kp_0 = 320,
			kp_1 = 321,
			kp_2 = 322,
			kp_3 = 323,
			kp_4 = 324,
			kp_5 = 325,
			kp_6 = 326,
			kp_7 = 327,
			kp_8 = 328,
			kp_9 = 329,
			kp_decimal = 330,
			kp_divide = 331,
			kp_multiply = 332,
			kp_subtract = 333,
			kp_add = 334,
			kp_enter = 335,
			kp_equal = 336,
			left_shift = 340,
			left_control = 341,
			left_alt = 342,
			left_super = 343,
			right_shift = 344,
			right_control = 345,
			right_alt = 346,
			right_super = 347,
			menu = 348,
			last = menu,
			scrolling = 349,
		};

		i32 scancode, mods;
		keycode key;
		action state;

		/**
		 * @brief Mouse click positions
		 */
		vec2<f64> pos;

		/**
		 * @brief Scrolling offsets
		 */
		vec2<f64> offset;
	};

	struct directional_keys
	{
		directional_keys();

		input::keycode up;
		input::keycode down;
		input::keycode left;
		input::keycode right;

		/**
		 * @brief Reset the keybinds to WASD
		 */
		void reset();

		/**
		 * @brief Use the vim movement keys
		 */
		void vim_preset();

		/**
		 * @brief Use arrow keys for movement
		 */
		void arrow_key_preset();
	};
}
