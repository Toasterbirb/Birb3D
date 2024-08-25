#pragma once

#include "Types.hpp"

#include <array>
#include <deque>

namespace birb
{
	class timestep
	{
	public:
		/**
		 * @brief Construct a timestep with target FPS set to the
		 * refreshrate of the primary monitor
		 */
		timestep();

		/**
		 * @brief Construct a timestep with a custom target FPS
		 *
		 * @param target_fps The amount of frames per second that the game tries to maintain
		 */
		explicit timestep(const f64 target_fps);

		/**
		 * @brief End a gameloop iteration and start a new one
		 *
		 * Call this function at the end of the gameloop. If the frametime duration
		 * the loop iteration was below the target frametime (frames per second)
		 * this function will stall the gameloop to maintain a stable framerate.
		 *
		 * @note This function also maintains the time reference for deltatime
		 * so forgetting to call this properly can make deltatime inaccurate
		 */
		void step();

		/**
		 * @return Time passed since startup in seconds
		 */
		f64 time_since_startup() const;

		/**
		 * @return The duration of the previous frame
		 */
		f64 deltatime() const;

		/**
		 * @return The duration of the previous frame as a float
		 */
		f32 deltatime_float() const;

		/**
		 * @brief Get the current FPS
		 *
		 * The framerate will be calculated from the frametime, so due to
		 * floating point accuracy, the end result may not be fully accurate
		 * at higher framerates.
		 *
		 * @return Amount of frames rendered per second at the time of calling
		 */
		f64 fps() const;

		/**
		 * @brief Calculate the portion of framebudget used
		 *
		 * Framebudget is the time we can be spend while
		 * still staying above the target framerate. This function calculates
		 * the amount we have used from this so called framebudget.
		 *
		 * @return Framebudget usage as a percentage value
		 */
		f64 framebudget() const;

		static constexpr u8 frametime_history_size = 240;

		/**
		 * @brief Frametime history for the past second (up-to 240 fps)
		 */
		std::array<f32, frametime_history_size> frametime_history = {1};

		/**
		 * @brief Set a new target framerate during runtime
		 *
		 * @param target_fps New target framerate as frames per second
		 */
		void set_target_fps(const f64 target_fps);

		/**
		 * @brief If set to true, the target FPS will be ignored
		 * and the framerate will be uncapped
		 */
		bool disable_fps_cap = false;

	private:
		/**
		 * @brief Initialize history arrays like frametime_history and framebudget_history
		 * with the correct sizes and data
		 */
		void setup_history_arrays();

		f64 frame_start = 0.0;
		f64 frame_end = 0.0;

		// Target FPS (frames per second)
		f64 target_fps = 60.0;

		// Target frametime (milliseconds per frame)
		f64 target_frametime = 16.7;

		u16 current_frametime_history_size = 1;

		f64 _deltatime = 0.001;

		// Framebudget usage of the previous frame
		static constexpr u16 framebudget_sample_count = 64;
		std::deque<f64> framebudget_history;
	};
}
