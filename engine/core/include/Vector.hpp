#pragma once

#include "Assert.hpp"
#include "Types.hpp"

#include <array>
#include <cmath>
#include <concepts>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

namespace birb
{
	template<class T>
	struct vec3;

	// 2D point
	template<class T>
	struct vec2
	{
		T x, y;

		constexpr vec2() : x(0), y(0) {};
		constexpr vec2(T x, T y) : x(x), y(y) {};
		constexpr vec2(T values[2]) : x(values[0]), y(values[1]) {};
		constexpr vec2(T value) : x(value), y(value) {};
		constexpr vec2(vec3<T> vector) : x(vector.x), y(vector.y) {};

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(x, y);
		}

		/// Returns the normalization of the vector
        constexpr vec2<T> normalized() const
		{
			ensure(magnitude() != 0, "Zero division");
			return *this / magnitude();
		}

		// Returns the magnitude of the vector
        constexpr f32 magnitude() const
		{
			return std::sqrt((x * x) + (y * y));
		}

		// Returns the vec3 in the following string form: "[x, y]"
		std::string to_string() const
		{
			return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
		}

		// Convert from any type to integer
		//
		// No rounding is done and floating point values will get floored
		constexpr vec2<i32> to_int() const
		{
			static_assert(!std::is_same<i32, T>::value, "Unnecessary conversion from vec2<i32> to vec2<i32>");
			return vec2<i32>(x, y);
		}

		// Convert from any type to float
		constexpr vec2<f32> to_float() const
		{
			static_assert(!std::is_same<f32, T>::value, "Unnecessary conversion from vec2<f32> to vec2<f32>");
			return vec2<f32>(x, y);
		}

		std::array<T*, 2> to_ptr_array()
		{
			return { &x, &y };
		}

		constexpr glm::vec2 to_glm_vec() const
		{
			return glm::vec2(x, y);
		}

		constexpr bool operator==(const vec2& other) const
		{
			return (this->x == other.x && this->y == other.y);
		}

		constexpr bool operator!=(const vec2& other) const
		{
			return (this->x != other.x || this->y != other.y);
		}

        constexpr vec2<T> operator+(const vec2<T>& other) const
        {
			return vec2<T>(x + other.x, y + other.y);
        }

        constexpr vec2<T>& operator+=(const vec2<T>& other)
        {
			x += other.x;
			y += other.y;
			return *this;
        }

        constexpr vec2<T> operator-(const vec2<T>& other) const
        {
			return vec2<T>(x - other.x, y - other.y);
        }

        constexpr vec2<T>& operator-=(const vec2<T>& other)
        {
			x -= other.x;
			y -= other.y;
			return *this;
        }

        constexpr vec2<T> operator*(const vec2<T>& other) const
        {
			return vec2<T>(x * other.x, y * other.y);
        }

        constexpr vec2<T>& operator*=(const vec2<T>& other)
        {
			x *= other.x;
			y *= other.y;
			return *this;
        }

        constexpr vec2<T> operator*(T other) const
        {
			return vec2<T>(x * other, y * other);
        }

        constexpr vec2<T> operator/(const vec2<T>& other) const
        {
			ensure(other.x != 0, "Zero division");
			ensure(other.y != 0, "Zero division");
			return vec2<T>(x / other.x, y / other.y);
        }

        constexpr vec2<T>& operator/=(const vec2<T>& other)
        {
			ensure(other.x != 0, "Zero division");
			ensure(other.y != 0, "Zero division");
			x /= other.x;
			y /= other.y;
			return *this;
        }

        constexpr vec2<T> operator/(T other) const
        {
			ensure(other != 0, "Zero division");
			return vec2<T>(x / other, y / other);
        }

		static constexpr birb::vec2<T> one()
		{
			return birb::vec2<T>(1,1);
		}

        static constexpr birb::vec2<T> zero()
		{
			return birb::vec2<T>(0,0);
		}

        static constexpr birb::vec2<T> up()
		{
			return birb::vec2<T>(0,1);
		}

        static constexpr birb::vec2<T> down()
		{
			static_assert(!std::unsigned_integral<T>, "Integer underflow");
			return birb::vec2<T>(0,-1);
		}

        static constexpr birb::vec2<T> left()
		{
			static_assert(!std::unsigned_integral<T>, "Integer underflow");
			return birb::vec2<T>(-1,0);
		}

        static constexpr birb::vec2<T> right()
		{
			return birb::vec2<T>(1,0);
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
		constexpr vec3(vec2<T> vec) : x(vec.x), y(vec.y), z(0) {};
		constexpr vec3(T value) : x(value), y(value), z(value) {};

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(x, y, z);
		}

		/// Returns the normalization of the vector
        constexpr vec3<T> normalized() const
		{
			ensure(magnitude() != 0, "Zero division");
			return *this / magnitude();
		}

		/// Returns the magnitude of the vector
        constexpr f32 magnitude() const
		{
			return std::sqrt((x * x) + (y * y) + (z * z));
		}

		// Returns the vec3 in the following string form: "[x, y, z]"
		std::string to_string() const
		{
			return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
		}

		// Convert from any type to integer
		//
		// No rounding is done and floating point values will get floored
		constexpr vec3<i32> to_int() const
		{
			static_assert(!std::is_same<i32, T>::value, "Unnecessary conversion from vec3<i32> to vec3<i32>");
			return vec3<i32>(x, y, z);
		}

		// Convert from any type to float
		constexpr vec3<f32> to_float() const
		{
			static_assert(!std::is_same<f32, T>::value, "Unnecessary conversion from vec3<f32> to vec3<f32>");
			return vec3<f32>(x, y, z);
		}

		std::array<T*, 3> to_ptr_array()
		{
			return { &x, &y, &z };
		}

		constexpr glm::vec3 to_glm_vec() const
		{
			return glm::vec3(x, y, z);
		}

		constexpr operator glm::vec3() const
		{
			return glm::vec3(x, y, z);
		}

		constexpr bool operator==(const vec3 other) const
		{
			return (this->x == other.x && this->y == other.y && this->z == other.z);
		}

		constexpr bool operator!=(const vec3& other) const
		{
			return (this->x != other.x || this->y != other.y || this->z != other.z);
		}

        constexpr vec3<T> operator+(const vec3<T>& other) const
        {
			return vec3<T>(x + other.x, y + other.y, z + other.z);
        }

        constexpr vec3<T>& operator+=(const vec3<T>& other)
        {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
        }

        constexpr vec3<T> operator-(const vec3<T>& other) const
        {
			return vec3<T>(x - other.x, y - other.y, z - other.z);
        }

        constexpr vec3<T>& operator-=(const vec3<T>& other)
        {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
        }

        constexpr vec3<T> operator*(const vec3<T>& other) const
        {
			return vec3<T>(x * other.x, y * other.y, z * other.z);
        }

        constexpr vec3<T>& operator*=(const vec3<T>& other)
        {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
        }

        constexpr vec3<T> operator*(T other) const
        {
			return vec3<T>(x * other, y * other, z * other);
        }

        constexpr vec3<T> operator/(const vec3<T>& other) const
        {
			ensure(other.x != 0, "Zero division");
			ensure(other.y != 0, "Zero division");
			ensure(other.z != 0, "Zero division");
			return vec3<T>(x / other.x, y / other.y, z / other.z);
        }

        constexpr vec3<T>& operator/=(const vec3<T>& other)
        {
			ensure(other.x != 0, "Zero division");
			ensure(other.y != 0, "Zero division");
			ensure(other.z != 0, "Zero division");
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
        }

        constexpr vec3<T> operator/(T other) const
        {
			ensure(other != 0, "Zero division");
			return vec3<T>(x / other, y / other, z / other);
        }

		static constexpr birb::vec3<T> one()
		{
			return birb::vec3<T>(1,1,1);
		}

        static constexpr birb::vec3<T> zero()
		{
			return birb::vec3<T>(0,0,0);
		}

        static constexpr birb::vec3<T> forward()
		{
			return birb::vec3<T>(0,0,1);
		}

        static constexpr birb::vec3<T> back()
		{
			static_assert(!std::unsigned_integral<T>, "Integer underflow");
			return birb::vec3<T>(0,0,-1);
		}

        static constexpr birb::vec3<T> up()
		{
			return birb::vec3<T>(0,1,0);
		}

        static constexpr birb::vec3<T> down()
		{
			static_assert(!std::unsigned_integral<T>, "Integer underflow");
			return birb::vec3<T>(0,-1,0);
		}

        static constexpr birb::vec3<T> left()
		{
			static_assert(!std::unsigned_integral<T>, "Integer underflow");
			return birb::vec3<T>(-1,0,0);
		}

        static constexpr birb::vec3<T> right()
		{
			return birb::vec3<T>(1,0,0);
		}
	};

	template<typename T>
	std::ostream &operator<<(std::ostream &stream, const vec2<T> &other)
    {
        stream << "[" << other.x << ", " << other.y << "]";
        return stream;
    }

	template<typename T>
	std::ostream &operator<<(std::ostream &stream, const vec3<T> &other)
    {
        stream << "[" << other.x << ", " << other.y << ", " << other.z << "]";
        return stream;
    }
}
