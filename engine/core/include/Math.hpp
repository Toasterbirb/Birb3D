#pragma once

#include "Transform.hpp"
#include "Vector.hpp"

#include <cmath>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
#include <type_traits>
#include <vector>

static constexpr f32 PI = 3.1415927;

namespace birb
{
	template<typename T>
	constexpr bool is_bit_set(T variable, u8 position)
	{
		static_assert(!std::is_same<T, bool>::value);
		static_assert(!std::is_same<T, f32>::value);
		static_assert(!std::is_same<T, f64>::value);
		return (variable & (1 << position)) != 0;
	}

	template<typename T>
	constexpr T combine_hashes(const T a, const T b)
	{
		static_assert(!std::is_same<T, f32>::value && !std::is_same<T, f64>::value, "Floating point values cannot be used as hashes");
		return a ^ (b << 1);
	}

	/**
	 * @brief Round a value down to a given accuracy
	 *
	 * @tparam T type
	 * @param value Value to be rounded
	 * @param decimal_points How many decimal points of accuracy should the value be rounded to
	 * @return Rounded value
	 */
	template<typename T>
	constexpr f64 round(const T value, const i8 decimal_points)
	{
		/* How this thing works:
		 * 1. Multiply the value with 10 ^ decimal points. This will leave the needed values before the decimal point
		 * 2. Round to integer
		 * 3. Divide the value with 10 ^ decimal points to get the desired rounded decimal value
		 * */
		return std::round(value * std::pow(10, decimal_points)) / static_cast<f64>(std::pow(10, decimal_points));
	}

	/**
	 * @brief Convert degrees to radians
	 */
	template<typename T>
	constexpr T deg_to_rad(const T degrees)
	{
		return degrees * (PI / 180.0f);
	}

	/**
	 * @brief Convert radians to degrees
	 */
	template<typename T>
	constexpr T rad_to_deg(const T radians)
	{
		return radians * (180.0f / PI);
	}

	/**
	 * @brief Calculate the average value of an array
	 */
	template<typename T, size_t N>
	constexpr T average(const std::array<T, N>& data)
	{
		static_assert(N > 0);

		T total = std::accumulate(data.begin(), data.end(), 0.0);
		return total / data.size();
	}

	/**
	 * @brief Calculate the average value of a 2D array
	 */
	template<typename T, size_t N, size_t M>
	constexpr T average(const std::array<std::array<T, N>, M>& data)
	{
		static_assert(N > 0);
		static_assert(M > 0);

		T total = 0;

		for (size_t i = 0; i < M; ++i)
			total += std::accumulate(data[i].begin(), data[i].end(), 0.0);

		return total / (N * M);
	}

	/**
	 * @brief Calculate the average value of a vector
	 */
	template<typename T>
	constexpr T average(const std::vector<T>& data)
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
	constexpr T average_deque(const std::deque<T>& data)
	{
		// Avoid a zero division
		if (data.empty())
			return 0;

		T total = std::accumulate(data.begin(), data.end(), 0.0);
		return total / data.size();
	}

	template<typename T, size_t N>
	constexpr T min_element(std::array<T, N> array)
	{
		static_assert(N > 0, "The array can't be empty");
		return *std::min_element(array.begin(), array.end());
	}

	template<typename T>
	constexpr T min_element(std::vector<T> vector)
	{
		ensure(!vector.empty(), "The vector can't be empty");
		return *std::min_element(vector.begin(), vector.end());
	}

	template<typename T, size_t N>
	constexpr T max_element(std::array<T, N> array)
	{
		static_assert(N > 0, "The array can't be empty");
		return *std::max_element(array.begin(), array.end());
	}

	template<typename T>
	constexpr T max_element(std::vector<T> vector)
	{
		ensure(!vector.empty(), "The vector can't be empty");
		return *std::max_element(vector.begin(), vector.end());
	}

	/**
	 * @brief Calculate the centerpoint between two 3D vectors
	 */
	template<typename T>
	constexpr vec3<T> center(const vec3<T> a, const vec3<T> b)
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
	constexpr T vec_distance(const vec3<T> a, const vec3<T> b)
	{
		return std::sqrt(
				std::pow(b.x - a.x, 2) +
				std::pow(b.y - a.y, 2) +
				std::pow(b.z - a.z, 2)
			);
	}

	namespace noise
	{
		f32 perlin(f32 x, f32 y, f32 z, u8 seed = 0);
		f32 perlin(vec2<f32> position, u8 seed = 0);
		f32 perlin(vec3<f32> position, u8 seed = 0);
	}

	namespace view_vector
	{
		glm::vec3 front(const transform& transform);
		glm::vec3 front(const f32 pitch, const f32 yaw);
		glm::vec3 right(const glm::vec3& front, const glm::vec3& world_up);
		glm::vec3 up(const glm::vec3& right, const glm::vec3& front);
	}
}
