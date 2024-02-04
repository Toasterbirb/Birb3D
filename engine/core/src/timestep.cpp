#include "Timestep.hpp"

#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

namespace birb
{
	timestep::timestep()
	{
		this->frame_end = glfwGetTime();

		// By default the target fps is set to the refreshrate
		// of the primary monitor
		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		set_target_fps(vid_mode->refreshRate);
	}

	timestep::timestep(double target_fps)
	{
		this->frame_end = glfwGetTime();
		set_target_fps(target_fps);
	}

	void timestep::step()
	{
		double frame_duration = deltatime() * 0.001;
		this->frame_end = glfwGetTime();

		// Delay to keep up the target framerate
		double delay_time = target_frametime - frame_duration;
		if (delay_time > 0)
		{
			std::chrono::duration<double> delay_duration(delay_time);
			std::this_thread::sleep_for(delay_duration);
		}
	}

	double timestep::deltatime() const
	{
		return glfwGetTime() - this->frame_end;
	}

	double timestep::fps() const
	{
		return 1.0 / deltatime();
	}

	void timestep::set_target_fps(double target_fps)
	{
		this->target_fps = target_fps;

		// Convert the target fps to frametime in milliseconds
		this->target_frametime = 1.0f / target_fps;
	}
}
