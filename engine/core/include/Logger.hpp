#pragma once

#include <iostream>
#include <string>

// Macro useful for marking "TODO" stuff in code that'll get logged during runtime as a reminder
// or as a notification of possibly missing features
#define FIXME(MESSAGE) std::cout << "\033[35m[F] " << MESSAGE << " @ " << __FILE__ << ":" << __LINE__ << "\033[0m\n";

namespace birb
{
	/**
	 * @brief Print debug level information
	 *
	 * Example output:
	 * 	[L] Value: 4
	 *
	 * @param label
	 * @param data
	 */
	template<typename T>
	static void log(const std::string& label, T data)
	{
		std::cout << "[L] " << label << ": " << data << "\n";
	}

	/**
	 * @brief Print debug level information
	 *
	 * @param text Text to print
	 */
	void log(const std::string& text);

	/**
	 * @brief Print a yellow warning message
	 *
	 * @param text Warning text to print
	 */
	void log_warn(const std::string& text);

	/**
	 * @brief Print a red error message
	 *
	 * @param text Error text to print
	 */
	void log_error(const std::string& text);

	/**
	 * @brief Print a fatal error and crash the program
	 *
	 * @param text Error text to print
	 * @param exit_code Exit code that will be used when quitting the program
	 */
	void log_fatal(const std::string& text, const int exit_code = 1);

	/**
	 * @brief Helper function for converting 64-bit pointers to a string
	 */
	std::string ptr_to_str(const void* ptr);
}
