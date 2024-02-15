#include "Timestep.hpp"

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

		// Delay to keep up the target framerate
		double delay_time = target_frametime - (_deltatime * 0.001);

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

	void timestep::set_target_fps(double target_fps)
	{
		assert(target_fps != 0 && "Zero division");

		this->target_fps = target_fps;

		// Convert the target fps to frametime in milliseconds
		target_frametime = 1.0 / target_fps;
	}
}
