#pragma once

#include <cmath>

namespace birb
{
	// Round a value down to a given accuracy
	template<typename T>
	constexpr double round(T value, int decimal_points)
	{
		/* How this thing works:
		 * 1. Multiply the value with 10 ^ decimal points. This will leave the needed values before the decimal point
		 * 2. Round to integer
		 * 3. Divide the value with 10 ^ decimal points to get the desired rounded decimal value
		 * */
		return std::round(value * std::pow(10, decimal_points)) / std::pow(10, decimal_points);
	}
}
