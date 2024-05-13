#include "Logger.hpp"

#include <stb_sprintf.h>

namespace birb
{
	std::string ptr_to_str(const void* ptr)
	{
		constexpr u8 buf_size = 18;
		char buf[buf_size];
		stbsp_snprintf(buf, buf_size, "%p", ptr);
		return "0x" + std::string(buf);
	}
}
