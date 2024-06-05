#pragma once

#include "Types.hpp"

#include <iostream>
#include <string>

#ifndef NDEBUG
#include <cpptrace/cpptrace.hpp>
#define PRINT_STACKTRACE() cpptrace::generate_trace().print_with_snippets();
#else
#define PRINT_STACKTRACE();
#endif


// Macro useful for marking "TODO" stuff in code that'll get logged during runtime as a reminder
// or as a notification of possibly missing features
#define FIXME(MESSAGE) std::cout << "\033[35m[F] " << MESSAGE << " @ " << __FILE__ << ":" << __LINE__ << "\033[0m\n";

namespace birb
{
	/**
	 * @brief Print debug level information
	 *
	 * @param args Text to print
	 */
	template<class... Args>
	void log(Args... args)
	{
		std::cout << "[L] ";
		(std::cout << ... << args) << "\n";
	}

	/**
	 * @brief Print a yellow warning message
	 *
	 * @param args Warning text to print
	 */
	template<class... Args>
	void log_warn(Args... args)
	{
		std::cout << "\033[33m[W] ";
		(std::cout << ... << args) << "\033[0m\n";
	}

	/**
	 * @brief Print a red error message
	 *
	 * @param args Error text to print
	 */
	template<class... Args>
	void log_error(Args... args)
	{
		std::cerr << "\033[31m[E] ";
		(std::cerr << ... << args) << "\033[0m\n";

		PRINT_STACKTRACE();
	}

	/**
	 * @brief Print a red error message without stacktrace
	 *
	 * @param args Error text to print
	 */
	template<class... Args>
	void log_error_no_trace(Args... args)
	{
		std::cerr << "\033[31m[E] ";
		(std::cerr << ... << args) << "\033[0m\n";
	}

	/**
	 * @brief Print a fatal error and crash the program
	 *
	 * @param exit_code Exit code that will be used when quitting the program
	 * @param args Error text to print
	 */
	template<class... Args>
	void log_fatal(const u8 exit_code, Args... args)
	{
		std::cerr << "\033[31mFATAL ERROR: ";
		(std::cerr << ... << args) << "\033[0m\n";

		PRINT_STACKTRACE();

		exit(exit_code);
	}

	/**
	 * @brief Helper function for converting 64-bit pointers to a string
	 */
	std::string ptr_to_str(const void* ptr);

	void process_gl_errors(const u32 stacktrace_scope_skip_amount = 0);
}
