#pragma once

#include "Vector.hpp"

namespace birb
{
	// 2D rectangle with a 2D position, width and height
	template<class T>
	struct rect
	{
		T x, y, w, h;

		constexpr rect() : x(0), y(0), w(0), h(0) {}
		constexpr rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
		constexpr rect(T values[4]) : x(values[0]), y(values[1]), w(values[2]), h(values[3]) {};

		constexpr vec2<float> center_point() const
		{
			return {x + w / 2.0f, y + h / 2.0f};
		}

		constexpr rect<int> to_int() const
		{
			return rect<int>(x, y, w, h);
		}

		constexpr rect<float> to_float() const
		{
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
