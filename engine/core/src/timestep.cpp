#include "Timestep.hpp"

#include <algorithm>
#include <cassert>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

namespace birb
{
	timestep::timestep()
	{
		frame_end = glfwGetTime();

		// By default the target fps is set to the refreshrate
		// of the primary monitor
		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		set_target_fps(vid_mode->refreshRate);
	}

	timestep::timestep(double target_fps)
	{
		frame_end = glfwGetTime();
		set_target_fps(target_fps);
	}

	void timestep::step()
	{
		double current_time = glfwGetTime();

		_deltatime = current_time - frame_end;
		frame_end = current_time;

		// Update the frametime history
		std::rotate(frametime_history.begin(), frametime_history.begin() + 1, frametime_history.end());
		frametime_history.at(frametime_history.size() - 1) = _deltatime;

		// Delay to keep up the target framerate
		double delay_time = target_frametime - (_deltatime * 0.001);
		previous_framebudget = 1 - (delay_time / target_frametime);

		if (delay_time > 0)
		{
			// Add a little bit of headroom to avoid going below the target FPS
			// This calculation uses the frametime to try to keep it accurate when
			// the FPS increases. A static multiplier doesn't scale too well
			// but this solution probably isn't the best one either
			delay_time *= 1 - target_frametime;

			std::chrono::duration<double> delay_duration(delay_time);
			std::this_thread::sleep_for(delay_duration);
		}
	}

	double timestep::deltatime() const
	{
		return _deltatime;
	}

	double timestep::fps() const
	{
		return 1.0 / deltatime();
	}

	double timestep::framebudget() const
	{
		return previous_framebudget;
	}

	void timestep::set_target_fps(double target_fps)
	{
		assert(target_fps != 0 && "Zero division");

		this->target_fps = target_fps;

		// Convert the target fps to frametime in milliseconds
		target_frametime = 1.0 / target_fps;

		// Update the frametime history size to fit one second worth of frametime data
		constexpr double min_frametime_history_size = 16;
		constexpr double max_frametime_history_size = 240;
		frametime_history.resize(std::clamp(target_fps, min_frametime_history_size, max_frametime_history_size));
	}
}
