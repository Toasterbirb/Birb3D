#include "Stopwatch.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

namespace birb
{
	stopwatch::stopwatch(const std::string& name)
	:name(name)
	{
		start = glfwGetTime();
	}

	stopwatch::~stopwatch()
	{
		if (!is_stopped)
			stop(false);
	}

	void stopwatch::reset()
	{
		is_stopped = false;
		start = glfwGetTime();
	}

	f64 stopwatch::stop(const bool quiet)
	{
		const f64 duration = glfwGetTime() - start;
		is_stopped = true;

		if (quiet)
			return duration;

		if (duration > 1.0)
			std::cout << text_color_code << name << ": " << duration << "s\033[0m\n";
		else if (duration > 0.001)
			std::cout << text_color_code << name << ": " << duration * 1'000.0 << "ms\033[0m\n";
		else if (duration > 0.000001)
			std::cout << text_color_code << name << ": " << duration * 1'000'000.0 << "µs\033[0m\n";
		else
			std::cout << text_color_code << name << ": " << duration * 1'000'000'000.0 << "ns\033[0m\n";

		return duration;
	}

	std::string stopwatch::format_time(const f64 seconds)
	{
		if (seconds > 1.0)
			return std::to_string(seconds) + "s";

		if (seconds > 0.001)
			return std::to_string(seconds * 1'000.0) + "ms";

		if (seconds > 0.000001)
			return std::to_string(seconds * 1'000'000.0) + "µs";

		return std::to_string(seconds * 1'000'000'000.0) + "ns";
	}
}
