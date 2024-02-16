#include "Timestep.hpp"
#include "Logger.hpp"

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

		// Fill frametime history with 1s to avoid zero divisions in the performance widget
		std::fill(frametime_history.begin(), frametime_history.end(), 1);
	}

	timestep::timestep(double target_fps)
	{
		frame_end = glfwGetTime();
		frame_start = glfwGetTime();
		set_target_fps(target_fps);
	}

	void timestep::step()
	{
		double current_time = glfwGetTime();
		double delay_time = target_frametime - (current_time - frame_start);

		_deltatime = current_time - frame_end;
		frame_end = current_time;

		// Update the frametime history
		std::rotate(frametime_history.begin(), frametime_history.begin() + 1, frametime_history.end());
		frametime_history.at(frametime_history.size() - 1) = _deltatime;

		// Delay to keep up the target framerate
		previous_framebudget = 1 - (delay_time / target_frametime);

		if (delay_time > 0)
		{
			std::chrono::duration<double> delay_duration(delay_time);
			std::this_thread::sleep_for(delay_duration);
		}

		frame_start = glfwGetTime();
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
		assert(target_fps > 0 && "Target FPS below zero is not valid");

		this->target_fps = target_fps;

		// Convert the target fps to frametime in milliseconds
		target_frametime = 1.0 / target_fps;
		assert(target_frametime != 0 && "Target frametime of zero will cause a zero division");

		// Update the frametime history size to fit one second worth of frametime data
		constexpr double min_frametime_history_size = 16;
		constexpr double max_frametime_history_size = 240;
		frametime_history.resize(std::clamp(target_fps, min_frametime_history_size, max_frametime_history_size));
	}
}
