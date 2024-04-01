#include "Math.hpp"
#include "Vector.hpp"

#include <doctest/doctest.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
	CHECK(vec2_a != vec2_b);

	birb::vec3<int> vec3_a(4, 12, 32);
	birb::vec3<int> vec3_b(12, 65, 32);
	CHECK(vec3_a == vec3_a);
	CHECK_FALSE(vec3_a == vec3_b);
	CHECK(vec3_a != vec3_b);
}

TEST_CASE("Vector conversions")
{
	SUBCASE("Int to float")
	{
		birb::vec2<int> vec2_a(4, 1);
		birb::vec2<float> vec2_b = vec2_a.to_float();
		CHECK(vec2_b.x == 4.0f);
		CHECK(vec2_b.y == 1.0f);

		birb::vec3<int> vec3_a(1, 2, 3);
		birb::vec3<float> vec3_b = vec3_a.to_float();
		CHECK(vec3_b.x == 1.0f);
		CHECK(vec3_b.y == 2.0f);
		CHECK(vec3_b.z == 3.0f);
	}

	SUBCASE("Float to int")
	{
		birb::vec2<float> vec2_a(4.0f, 1.5f);
		birb::vec2<int> vec2_b = vec2_a.to_int();
		CHECK(vec2_b.x == 4);
		CHECK(vec2_b.y == 1);

		birb::vec3<float> vec3_a(1.0f, 2.0f, 3.0f);
		birb::vec3<int> vec3_b = vec3_a.to_int();
		CHECK(vec3_b.x == 1);
		CHECK(vec3_b.y == 2);
		CHECK(vec3_b.z == 3);
	}

	SUBCASE("vec to glm::vec")
	{
		birb::vec2<float> vec2(4.0f, 5.0f);
		glm::vec2 glm_vec2 = vec2.to_glm_vec();
		CHECK(glm_vec2.x == vec2.x);
		CHECK(glm_vec2.y == vec2.y);

		birb::vec3<float> vec3(4.0f, 5.0f, 6.0f);
		glm::vec3 glm_vec3 = vec3.to_glm_vec();
		CHECK(glm_vec3.x == vec3.x);
		CHECK(glm_vec3.y == vec3.y);
		CHECK(glm_vec3.z == vec3.z);
	}
}

TEST_CASE("Vector magnitude")
{
	birb::vec2<int> vec_a(2, 3);
	birb::vec3<int> vec_b(2, 3, 4);
	birb::vec3<float> vec_c(2.0f, 3.0f, 4.0f);
	birb::vec2<float> vec_d(2.0f, 3.0f);

	CHECK(birb::round(vec_a.magnitude(), 2) == 3.61);
	CHECK(birb::round(vec_b.magnitude(), 2) == 5.39);
	CHECK(birb::round(vec_c.magnitude(), 2) == 5.39);
	CHECK(birb::round(vec_d.magnitude(), 2) == 3.61);
}

TEST_CASE("Vector normalize")
{
	birb::vec3<float> vec_a(2.0f, 3.0f, 4.0f);
	birb::vec2<float> vec_b(2.0f, 3.0f);

	CHECK(birb::round(vec_a.normalized().x, 2) == 0.37);
	CHECK(birb::round(vec_a.normalized().y, 2) == 0.56);
	CHECK(birb::round(vec_a.normalized().z, 2) == 0.74);

	CHECK(birb::round(vec_b.normalized().x, 2) == 0.55);
	CHECK(birb::round(vec_b.normalized().y, 2) == 0.83);
}

TEST_CASE("vec2 operator overloads")
{
	birb::vec2<int> vec_a(4, 2);
	birb::vec2<int> vec_b(2, 3);
	birb::vec2<int> vec_c(4, 3);
	birb::vec2<int> vec_d(2, 2);

	CHECK(vec_a + vec_b == birb::vec2<int>(6, 5));
	CHECK(vec_a - vec_b == birb::vec2<int>(2, -1));
	CHECK(vec_a * vec_b == birb::vec2<int>(8, 6));
	CHECK(vec_a / vec_b == birb::vec2<int>(2, 0));

	CHECK(vec_d * 2  == birb::vec2<int>(4, 4));
	CHECK(vec_d / 2.0f  == birb::vec2<int>(1, 1));

	CHECK(vec_a == vec_a);
	CHECK(vec_a != vec_b);
	CHECK(vec_a != vec_c);

	birb::vec2<int> vec_e(1, 2);
	birb::vec2<int> vec_f(2, 1);

	vec_e += vec_f;
	CHECK(vec_e == birb::vec2<int>(3, 3));

	vec_e -= vec_f;
	CHECK(vec_e == birb::vec2<int>(1, 2));

	vec_e *= vec_f;
	CHECK(vec_e == birb::vec2<int>(2, 2));

	vec_e /= vec_f;
	CHECK(vec_e == birb::vec2<int>(1, 2));
}

TEST_CASE("vec3 operator overloads")
{
	birb::vec3<int> vec_a(4, 6, 5);
	birb::vec3<int> vec_b(2, 3, 2);
	birb::vec3<int> vec_c(4, 7, 5);
	birb::vec3<int> vec_d(2, 2, 2);

	CHECK(vec_a + vec_b == birb::vec3<int>(6, 9, 7));
	CHECK(vec_a - vec_b == birb::vec3<int>(2, 3, 3));
	CHECK(vec_a * vec_b == birb::vec3<int>(8, 18, 10));
	CHECK(vec_a / vec_b == birb::vec3<int>(2, 2, 2));

	CHECK(vec_d * 2  == birb::vec3<int>(4, 4, 4));
	CHECK(vec_d / 2  == birb::vec3<int>(1, 1, 1));

	CHECK(vec_a == vec_a);
	CHECK(vec_a != vec_b);
	CHECK(vec_a != vec_c);

	birb::vec3<int> vec_e(1, 2, 1);
	birb::vec3<int> vec_f(2, 1, 2);

	vec_e += vec_f;
	CHECK(vec_e == birb::vec3<int>(3, 3, 3));

	vec_e -= vec_f;
	CHECK(vec_e == birb::vec3<int>(1, 2, 1));

	vec_e *= vec_f;
	CHECK(vec_e == birb::vec3<int>(2, 2, 2));

	vec_e /= vec_f;
	CHECK(vec_e == birb::vec3<int>(1, 2, 1));
}

TEST_CASE("vec3 from vec2")
{
	birb::vec3<i32> vec = birb::vec2<i32>(1, 2);
	CHECK(vec.x == 1);
	CHECK(vec.y == 2);
	CHECK(vec.z == 0);
}
