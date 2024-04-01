#pragma once

#include "Vector.hpp"
#include <type_traits>

namespace birb
{
	// 2D rectangle with a 2D position, width and height
	template<class T>
	struct rect
	{
		T x; ///< x-position
		T y; ///< y-position
		T w; ///< width
		T h; ///< height

		/**
		 * @brief Construct a rect with all values set to zero
		 */
		constexpr rect() : x(0), y(0), w(0), h(0) {}

		/**
		 * @brief Construct a rect with given values
		 *
		 * @param x x-position
		 * @param y y-position
		 * @param w width
		 * @param h height
		 */
		constexpr rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}

		/**
		 * @brief Construct a rect from an array with 4 elements
		 *
		 * @param values An array with the size of 4 that contains the values in the following order: [x-position, y-position, width, height]
		 */
		constexpr rect(T values[4]) : x(values[0]), y(values[1]), w(values[2]), h(values[3]) {};

		/**
		 * @brief Calculate the center point of the rect based on its position and dimensions
		 *
		 * @return 2D-point at the center of the rect
		 */
		constexpr vec2<float> center_point() const
		{
			return {x + w / 2.0f, y + h / 2.0f};
		}

		/**
		 * @brief Convert the rect into a rect with values stored as integers
		 *
		 * @warning  The converted rect will be constructed with no rounding and thus
		 * any floating point values will get floored.
		 *
		 * @return A new rect constructed as \ref birb::rect<i32>
		 */
		constexpr rect<i32> to_int() const
		{
			static_assert(!std::is_same<i32, T>::value, "Unnecessary conversion from rect<i32> to rect<i32>");
			return rect<i32>(x, y, w, h);
		}

		/**
		 * @brief Convert the rect into a rect with values stored as floats
		 *
		 * @return A new rect constructed as \ref birb::rect<float>
		 */
		constexpr rect<float> to_float() const
		{
			static_assert(!std::is_same<float, T>::value, "Unnecessary conversion from rect<float> to rect<float>");
			return rect<float>(x, y, w, h);
		}

		constexpr bool operator==(const rect& other) const
		{
			return (this->x == other.x && this->y == other.y && this->w == other.w && this->h == other.h);
		}

		constexpr bool operator!=(const rect& other) const
		{
			return (this->x != other.x || this->y != other.y || this->w != other.w || this->h != other.h);
		}
	};

	template<typename T>
	std::ostream &operator<<(std::ostream &stream, const rect<T> &other)
    {
        stream << "[" << other.x << ", " << other.y << ", " << other.w << ", " << other.h << "]";
        return stream;
    }
}
