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
