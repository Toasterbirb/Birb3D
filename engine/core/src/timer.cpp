#include "Timer.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

namespace birb
{
	timer::timer(const std::string& name)
	:name(name)
	{
		start = glfwGetTime();
	}

	timer::~timer()
	{
		if (!is_stopped)
			stop();
	}

	void timer::reset()
	{
		is_stopped = false;
		start = glfwGetTime();
	}

	void timer::stop()
	{
		const double duration = glfwGetTime() - start;
		is_stopped = true;

		if (duration > 1.0)
			std::cout << name << ": " << duration << "s\n";
		else
			std::cout << name << ": " << duration * 1000.0 << "ms\n";
	}
}