#pragma once

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
	};

	// 3D point
	template<class T>
	struct vec3
	{
		T x, y, z;

		constexpr vec3() : x(0), y(0), z(0) {};
		constexpr vec3(T x, T y, T z) : x(x), y(y), z(z) {};
		constexpr vec3(T values[3]) : x(values[0]), y(values[1]), z(values[2]) {};
	};
}
