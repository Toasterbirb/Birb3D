#pragma once

#include <string>
#include <iostream>

// Macro useful for marking "TODO" stuff in code that'll get logged during runtime as a reminder
// or as a notification of possibly missing features
#define FIXME(MESSAGE) std::cout << "\033[35m[F] " << MESSAGE << " @ " << __FILE__ << ":" << __LINE__ << "\033[0m\n";

namespace birb
{
	/**
	 * @brief Print debug level information
	 *
	 * @param text Text to print
	 */
	static void log(const std::string& text)
	{
		std::cout << "[L] " << text << "\n";
	}

	/**
	 * @brief Print a yellow warning message
	 *
	 * @param text Warning text to print
	 */
	static void log_warn(const std::string& text)
	{
		std::cout << "\033[33m[W] " << text << "\033[0m\n";
	}

	/**
	 * @brief Print a red error message
	 *
	 * @param text Error text to print
	 */
	static void log_error(const std::string& text)
	{
		std::cerr << "\033[31m[E] " << text << "\033[0m\n";
	}

	/**
	 * @brief Print a fatal error and crash the program
	 *
	 * @param text Error text to print
	 * @param exit_code Exit code that will be used when quitting the program
	 */
	static void log_fatal(const std::string& text, const int exit_code = 1)
	{
		std::cerr << "\033[31mFATAL ERROR: " << text << "\033[0m\n";
		exit(exit_code);
	}

	/**
	 * @brief Helper function for converting 64-bit pointers to a string
	 */
	static std::string ptr_to_str(const void* ptr)
	{
		return "0x" + std::to_string(reinterpret_cast<long long>(ptr));
	}
}
