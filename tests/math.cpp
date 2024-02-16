#include "Math.hpp"

#include <doctest/doctest.h>

TEST_CASE("Rounding with specified accuracy")
{
	double value1 = 0.013333333;
	float value2 = 0.017777777;

	CHECK(birb::round(value1, 6) == 0.013333);
	CHECK(birb::round(value1, 3) == 0.013);
	CHECK(birb::round(value1, 2) == 0.01);

	CHECK(birb::round(value2, 6) == 0.017778);
	CHECK(birb::round(value2, 3) == 0.018);
	CHECK(birb::round(value2, 2) == 0.02);
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
