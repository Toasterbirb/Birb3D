#pragma once

namespace birb
{
	// 2D point
	template<class T>
	struct vec2
	{
		T x, y;

		vec2() : x(0), y(0) {};
		vec2(T x, T y) : x(x), y(y) {};
		vec2(T values[2]) : x(values[0]), y(values[1]) {};
	};

	// 3D point
	template<class T>
	struct vec3
	{
		T x, y, z;

		vec3() : x(0), y(0), z(0) {};
		vec3(T x, T y, T z) : x(x), y(y), z(z) {};
		vec3(T values[3]) : x(values[0]), y(values[1]), z(values[2]) {};
	};
}
