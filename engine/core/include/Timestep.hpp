#pragma once

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
		timestep(double target_fps);

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
		 * @return The duration of the previous frame
		 */
		double deltatime() const;

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

	private:
		double frame_start;
		double frame_end;

		// Target FPS (frames per second)
		double target_fps;

		// Target frametime (milliseconds per frame)
		double target_frametime;

		double _deltatime;

		// Framebudget usage of the previous frame
		double previous_framebudget;
	};
}
