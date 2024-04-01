#pragma once

#include <cmath>
#include <deque>
#include <numeric>
#include <vector>

namespace birb
{
	/**
	 * @brief Round a value down to a given accuracy
	 *
	 * @tparam T type
	 * @param value Value to be rounded
	 * @param decimal_points How many decimal points of accuracy should the value be rounded to
	 * @return Rounded value
	 */
	template<typename T>
	constexpr double round(T value, i8 decimal_points)
	{
		/* How this thing works:
		 * 1. Multiply the value with 10 ^ decimal points. This will leave the needed values before the decimal point
		 * 2. Round to integer
		 * 3. Divide the value with 10 ^ decimal points to get the desired rounded decimal value
		 * */
		return std::round(value * std::pow(10, decimal_points)) / static_cast<double>(std::pow(10, decimal_points));
	}

	/**
	 * @brief Calculate the average value of a vector
	 */
	template<typename T>
	constexpr T average(std::vector<T> data)
	{
		// Avoid a zero division
		if (data.empty())
			return 0;

		T total = std::accumulate(data.begin(), data.end(), 0.0);
		return total / data.size();
	}

	/**
	 * @brief Calculate the average value of a dequeu
	 */
	template<typename T>
	constexpr T average_deque(std::deque<T> data)
	{
		// Avoid a zero division
		if (data.empty())
			return 0;

		T total = std::accumulate(data.begin(), data.end(), 0.0);
		return total / data.size();
	}
}
