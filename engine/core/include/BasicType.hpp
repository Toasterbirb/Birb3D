#pragma once

#include <string>

namespace birb
{
	template<typename T>
	struct basic_type
	{
		virtual std::string to_string() const = 0;
	};
}
