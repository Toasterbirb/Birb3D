#pragma once

#include <deque>
#include <vector>

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
		explicit timestep(double target_fps);

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
		double time_since_startup() const;

		/**
		 * @return The duration of the previous frame
		 */
		double deltatime() const;

		/**
		 * @return The duration of the previous frame as a float
		 */
		float deltatime_float() const;

		/**
		 * @brief Get the current FPS
		 *
		 * The framerate will be calculated from the frametime, so due to
		 * floating point accuracy, the end result may not be fully accurate
		 * at higher framerates.
		 *
		 * @return Amount of frames rendered per second at the time of calling
		 */
		double fps() const;

		/**
		 * @brief Calculate the portion of framebudget used
		 *
		 * Framebudget is the time we can be spend while
		 * still staying above the target framerate. This function calculates
		 * the amount we have used from this so called framebudget.
		 *
		 * @return Framebudget usage as a percentage value
		 */
		double framebudget() const;

		/**
		 * @brief Frametime history for the past second (up-to 240 fps)
		 */
		std::vector<float> frametime_history;

		/**
		 * @brief Set a new target framerate during runtime
		 *
		 * @param target_fps New target framerate as frames per second
		 */
		void set_target_fps(double target_fps);

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

		double frame_start = 0.0;
		double frame_end = 0.0;

		// Target FPS (frames per second)
		double target_fps = 60.0;

		// Target frametime (milliseconds per frame)
		double target_frametime = 16.7;

		double _deltatime = 0.001;

		// Framebudget usage of the previous frame
		static constexpr u16 framebudget_sample_count = 64;
		std::deque<double> framebudget_history;
	};
}
