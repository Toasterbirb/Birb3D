#include "Vector.hpp"

#include <doctest/doctest.h>

TEST_CASE("Vector constructors")
{
	constexpr int int_x = -23;
	constexpr int int_y = 53;
	constexpr int int_z = 983;

	constexpr float float_x = 35.53f;
	constexpr float float_y = -125.03f;
	constexpr float float_z = 9283.32f;

	constexpr double double_x = 12.543;
	constexpr double double_y = 83.23523;
	constexpr double double_z = 239.0398;

	SUBCASE("vec2 int")
	{
		birb::vec2<int> vec2_int(int_x, int_y);
		CHECK(vec2_int.x == int_x);
		CHECK(vec2_int.y == int_y);

		birb::vec2<int> vec2_int_arr({int_x, int_y});
		CHECK(vec2_int_arr.x == int_x);
		CHECK(vec2_int_arr.y == int_y);
	}

	SUBCASE("vec2 float")
	{
		birb::vec2<float> vec2_float(float_x, float_y);
		CHECK(vec2_float.y == float_y);
		CHECK(vec2_float.x == float_x);

		birb::vec2<float> vec2_float_arr({float_x, float_y});
		CHECK(vec2_float_arr.y == float_y);
		CHECK(vec2_float_arr.x == float_x);
	}

	SUBCASE("vec2 double")
	{
		birb::vec2<double> vec2_double(double_x, double_y);
		CHECK(vec2_double.x == double_x);
		CHECK(vec2_double.y == double_y);

		birb::vec2<double> vec2_double_arr({double_x, double_y});
		CHECK(vec2_double_arr.x == double_x);
		CHECK(vec2_double_arr.y == double_y);
	}

	SUBCASE("vec3 int")
	{
		birb::vec3<int> vec3_int(int_x, int_y, int_z);
		CHECK(vec3_int.x == int_x);
		CHECK(vec3_int.y == int_y);
		CHECK(vec3_int.z == int_z);

		birb::vec3<int> vec3_int_arr({int_x, int_y, int_z});
		CHECK(vec3_int_arr.x == int_x);
		CHECK(vec3_int_arr.y == int_y);
		CHECK(vec3_int_arr.z == int_z);
	}

	SUBCASE("vec3 float")
	{
		birb::vec3<float> vec3_float(float_x, float_y, float_z);
		CHECK(vec3_float.x == float_x);
		CHECK(vec3_float.y == float_y);
		CHECK(vec3_float.z == float_z);

		birb::vec3<float> vec3_float_arr({float_x, float_y, float_z});
		CHECK(vec3_float_arr.x == float_x);
		CHECK(vec3_float_arr.y == float_y);
		CHECK(vec3_float_arr.z == float_z);
	}

	SUBCASE("vec3 double")
	{
		birb::vec3<double> vec3_double(double_x, double_y, double_z);
		CHECK(vec3_double.x == double_x);
		CHECK(vec3_double.y == double_y);
		CHECK(vec3_double.z == double_z);

		birb::vec3<double> vec3_double_arr({double_x, double_y, double_z});
		CHECK(vec3_double_arr.x == double_x);
		CHECK(vec3_double_arr.y == double_y);
		CHECK(vec3_double_arr.z == double_z);
	}
}

TEST_CASE("Vector comparisons")
{
	birb::vec2<int> vec2_a(4, 12);
	birb::vec2<int> vec2_b(63, -42);
	CHECK(vec2_a == vec2_a);
	CHECK_FALSE(vec2_a == vec2_b);

	birb::vec3<int> vec3_a(4, 12, 32);
	birb::vec3<int> vec3_b(12, 65, 32);
	CHECK(vec3_a == vec3_a);
	CHECK_FALSE(vec3_a == vec3_b);
}
