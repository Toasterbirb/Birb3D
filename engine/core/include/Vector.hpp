#pragma once

#include <string>

namespace birb
{
	// 2D point
	template<class T>
	struct vec2
	{
		T x, y;

		constexpr vec2() : x(0), y(0) {};
		constexpr vec2(T x, T y) : x(x), y(y) {};
		constexpr vec2(T values[2]) : x(values[0]), y(values[1]) {};

		// Returns the vec3 in the following string form: "[x, y]"
		std::string to_string() const
		{
			return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
		}

		// Convert from any type to integer
		//
		// No rounding is done and floating point values will get floored
		constexpr vec2<int> to_int() const
		{
			return vec2<int>(this->x, this->y);
		}

		// Convert from any type to float
		constexpr vec2<float> to_float() const
		{
			return vec2<float>(this->x, this->y);
		}

		constexpr bool operator==(const vec2 other) const
		{
			return (this->x == other.x && this->y == other.y);
		}

		constexpr bool operator!=(const vec2& other) const
		{
			return (this->x != other.x || this->y != other.y);
		}

	};

	// 3D point
	template<class T>
	struct vec3
	{
		T x, y, z;

		constexpr vec3() : x(0), y(0), z(0) {};
		constexpr vec3(T x, T y, T z) : x(x), y(y), z(z) {};
		constexpr vec3(T values[3]) : x(values[0]), y(values[1]), z(values[2]) {};

		// Returns the vec3 in the following string form: "[x, y, z]"
		std::string to_string() const
		{
			return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
		}

		// Convert from any type to integer
		//
		// No rounding is done and floating point values will get floored
		constexpr vec3<int> to_int() const
		{
			return vec3<int>(this->x, this->y, this->z);
		}

		// Convert from any type to float
		constexpr vec3<float> to_float() const
		{
			return vec3<float>(this->x, this->y, this->z);
		}

		constexpr bool operator==(const vec3 other) const
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z);
		}

		constexpr bool operator!=(const vec3& other) const
		{
			return (this->x != other.x || this->y != other.y || this->z != other.z);
		}
	};
}
