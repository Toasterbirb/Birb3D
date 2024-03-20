#include "Logger.hpp"

#include <stb_sprintf.h>

namespace birb
{
	void log(const std::string& text)
	{
		std::cout << "[L] " << text << "\n";
	}

	void log_warn(const std::string& text)
	{
		std::cout << "\033[33m[W] " << text << "\033[0m\n";
	}

	void log_error(const std::string& text)
	{
		std::cerr << "\033[31m[E] " << text << "\033[0m\n";
	}

	void log_fatal(const std::string& text, const int exit_code)
	{
		std::cerr << "\033[31mFATAL ERROR: " << text << "\033[0m\n";
		exit(exit_code);
	}

	std::string ptr_to_str(const void* ptr)
	{
		constexpr unsigned int buf_size = 18;
		char buf[buf_size];
		stbsp_snprintf(buf, buf_size, "%p", ptr);
		return "0x" + std::string(buf);
	}
}
