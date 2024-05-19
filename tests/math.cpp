#include "Math.hpp"

#include <doctest/doctest.h>

TEST_CASE("Rounding with specified accuracy")
{
	const double value1 = 0.013333333;
	const float value2 = 0.017777777;
	const int value3 = 5;
	const int value4 = 4;

	CHECK(birb::round(value1, 6) == 0.013333);
	CHECK(birb::round(value1, 3) == 0.013);
	CHECK(birb::round(value1, 2) == 0.01);

	CHECK(birb::round(value2, 6) == 0.017778);
	CHECK(birb::round(value2, 3) == 0.018);
	CHECK(birb::round(value2, 2) == 0.02);

	CHECK(birb::round(value3, -1) == 10);
	CHECK(birb::round(value4, -1) == 0);
}

TEST_CASE("Radians to degrees")
{
	CHECK(birb::round(birb::rad_to_deg(1.0f), 4) == 57.2958);
	CHECK(birb::round(birb::rad_to_deg(2.0f), 4) == 114.5916);
}

TEST_CASE("Degrees to radians")
{
	CHECK(birb::round(birb::deg_to_rad(45.0f), 4) == 0.7854);
	CHECK(birb::round(birb::deg_to_rad(100.0f), 4) == 1.7453);
}

TEST_CASE("Calculate the average value of a std::array")
{
	std::array<int, 5> int_vals = {1, 2, 3, 4, 5};
	CHECK(birb::average(int_vals) == 3);

	std::array<float, 2> float_vals = {1.2f, 3.2f};
	CHECK(birb::average(float_vals) == 2.2f);
}

TEST_CASE("Calculate the average value of a 2D std::array")
{
	std::array<std::array<float, 3>, 3> values = {
		std::array<float, 3>{ 1.0f, 2.0f, 3.0 },
		std::array<float, 3>{ 2.0f, 3.0f, 4.0 },
		std::array<float, 3>{ 3.0f, 4.0f, 5.0 },
	};

	CHECK(birb::average(values) == 3.0f);
}

TEST_CASE("Calculate the average value of a std::vector")
{
	std::vector<int> int_vals = {1, 2, 3, 4, 5};
	CHECK(birb::average(int_vals) == 3);

	std::vector<float> float_vals = {1.2f, 3.2f};
	CHECK(birb::average(float_vals) == 2.2f);
}

TEST_CASE("Calculate the average value of a std::deque")
{
	std::deque<int> int_vals = {1, 2, 3, 4, 5};
	CHECK(birb::average_deque(int_vals) == 3);

	std::deque<float> float_vals = {1.2f, 3.2f};
	CHECK(birb::average_deque(float_vals) == 2.2f);
}

TEST_CASE("Calculate the centerpoint between two 3D float vectors")
{
	birb::vec3<float> point_a(1.0f, 1.0f, 1.0f);
	birb::vec3<float> point_b(8.0f, 3.0f, 2.0f);

	birb::vec3<float> result = birb::center(point_a, point_b);
	birb::vec3<float> expected_result(4.5f, 2.0f, 1.5f);

	CHECK(result == expected_result);
}

TEST_CASE("Distance calculation with 3D vectors")
{
	birb::vec3<float> point_a(1.0f, 1.0f, 1.0f);
	birb::vec3<float> point_b(8.0f, 3.0f, 2.0f);

	birb::vec3<int> point_a_int(1, 1, 1);
	birb::vec3<int> point_b_int(8, 3, 2);

	CHECK(std::roundf(birb::vec_distance(point_a, point_b)) == std::roundf(7.3484792283495));
	CHECK(std::roundf(birb::vec_distance(point_a_int, point_b_int)) == std::roundf(7.3484692283495));
}
