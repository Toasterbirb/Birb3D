#pragma once

#include <string>
#include <iostream>

namespace birb
{
	// Print text to the console
	static void log(const std::string& text)
	{
		std::cout << "[L] " << text << "\n";
	}

	static void log_warn(const std::string& text)
	{
		std::cout << "\033[33m[W] " << text << "\033[0m\n";
	}

	static void log_error(const std::string& text)
	{
		std::cout << "\033[31m[E] " << text << "\033[0m\n";
	}

	static void log_fatal(const std::string& text)
	{
		std::cout << "\033[31mFATAL ERROR: " << text << "\033[0m\n";
		exit(1);
	}
}
