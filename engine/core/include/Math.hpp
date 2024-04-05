#pragma once

#include "Transform.hpp"
#include "Vector.hpp"

#include <cmath>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	 * @brief Calculate the average value of an array
	 */
	template<typename T, size_t N>
	constexpr T average(std::array<T, N> data)
	{
		static_assert(N > 0);

		T total = std::accumulate(data.begin(), data.end(), 0.0);
		return total / data.size();
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

	/**
	 * @brief Calculate the centerpoint between two 3D vectors
	 */
	template<typename T>
	constexpr vec3<T> center(vec3<T> a, vec3<T> b)
	{
		return vec3<T>(
				(a.x + b.x) / 2.0,
				(a.y + b.y) / 2.0,
				(a.z + b.z) / 2.0
			);
	}

	/**
	 * @brief Calculate the distance between two 3D vectors
	 */
	template<typename T>
	constexpr T vec_distance(vec3<T> a, vec3<T> b)
	{
		return std::sqrt(
				std::pow(b.x - a.x, 2) +
				std::pow(b.y - a.y, 2) +
				std::pow(b.z - a.z, 2)
			);
	}

	namespace view_vector
	{
		glm::vec3 front(const component::transform& transform);
		glm::vec3 front(const float pitch, const float yaw);
		glm::vec3 right(const glm::vec3& front, const glm::vec3& world_up);
		glm::vec3 up(const glm::vec3& right, const glm::vec3& front);
	}
}
