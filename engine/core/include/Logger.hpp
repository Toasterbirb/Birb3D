#pragma once

#include <string>
#include <iostream>

namespace birb
{
	// Print debug level information
	static void log(const std::string& text)
	{
		std::cout << "[L] " << text << "\n";
	}

	// Print a yellow warning message
	static void log_warn(const std::string& text)
	{
		std::cout << "\033[33m[W] " << text << "\033[0m\n";
	}

	// Print a red error message
	static void log_error(const std::string& text)
	{
		std::cout << "\033[31m[E] " << text << "\033[0m\n";
	}

	// Print a fatal error and crash
	static void log_fatal(const std::string& text, const int exit_code = 1)
	{
		std::cout << "\033[31mFATAL ERROR: " << text << "\033[0m\n";
		exit(exit_code);
	}
}
