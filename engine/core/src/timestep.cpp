#include "Timestep.hpp"
#include "Math.hpp"
#include "Profiling.hpp"

#include <algorithm>
#include <cassert>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <microprofile.h>

#define PROFILER_GROUP "Timestep"
#define PROFILER_COLOR MP_LIGHTGREEN

namespace birb
{
	timestep::timestep()
	{
		frame_end = glfwGetTime();

		// By default the target fps is set to the refreshrate
		// of the primary monitor
		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		set_target_fps(vid_mode->refreshRate);
		setup_history_arrays();
	}

	timestep::timestep(double target_fps)
	{
		frame_end = glfwGetTime();
		frame_start = glfwGetTime();
		set_target_fps(target_fps);
		setup_history_arrays();
	}

	void timestep::setup_history_arrays()
	{
		PROFILER_SCOPE_RENDER_FN()
		// Fill frametime history with 1s to avoid zero divisions in the performance widget
		std::fill(frametime_history.begin(), frametime_history.end(), 1);

		// Resize the framebudget history to the sample count
		static_assert(birb::timestep::framebudget_sample_count > 0);
		framebudget_history.resize(birb::timestep::framebudget_sample_count);
	}

	void timestep::step()
	{
		PROFILER_SCOPE_RENDER_FN()
		double current_time = glfwGetTime();
		double delay_time = target_frametime - (current_time - frame_start);

		_deltatime = current_time - frame_end;
		frame_end = current_time;

		// Update the frametime history
		std::rotate(frametime_history.begin(), frametime_history.begin() + 1, frametime_history.end());
		frametime_history.at(frametime_history.size() - 1) = _deltatime;

		// Update framebudget history
		assert(!framebudget_history.empty());
		framebudget_history.pop_front();

		assert(target_frametime != 0 && "Zero division");
		framebudget_history.push_back(1 - (delay_time / target_frametime));

		// Delay to keep up the target framerate
		if (delay_time > 0 && !disable_fps_cap)
		{
			PROFILER_SCOPE_RENDER("Do nothing");
			std::chrono::duration<double> delay_duration(delay_time);
			std::this_thread::sleep_for(delay_duration);
		}

		frame_start = glfwGetTime();
	}

	double timestep::time_since_startup() const
	{
		return glfwGetTime();
	}

	double timestep::deltatime() const
	{
		return _deltatime;
	}

	float timestep::deltatime_float() const
	{
		return static_cast<float>(_deltatime);
	}

	double timestep::fps() const
	{
		assert(_deltatime != 0 && "Zero division");
		return 1.0 / _deltatime;
	}

	double timestep::framebudget() const
	{
		return birb::average_deque(framebudget_history);
	}

	void timestep::set_target_fps(double target_fps)
	{
		assert(target_fps != 0 && "Zero division");
		assert(target_fps > 0 && "Target FPS below zero is not valid");

		this->target_fps = target_fps;

		// Convert the target fps to frametime in milliseconds
		target_frametime = 1.0 / target_fps;
		assert(target_frametime != 0 && "Target frametime of zero will cause a zero division");

	}
}
