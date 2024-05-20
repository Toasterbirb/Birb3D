#include "Color.hpp"
#include "Math.hpp"

#include <doctest/doctest.h>

TEST_CASE("Color constructor")
{
	SUBCASE("Default")
	{
		birb::color color;
		CHECK(color.r == 0);
		CHECK(color.g == 0);
		CHECK(color.b == 0);
		CHECK(color.a == 1.0f);
	}

	SUBCASE("Float")
	{
		birb::color color(0.1f, 0.2f, 0.3f, 0.4f);
		CHECK(color.r == 0.1f);
		CHECK(color.g == 0.2f);
		CHECK(color.b == 0.3f);
		CHECK(color.a == 0.4f);
	}

	SUBCASE("Double")
	{
		birb::color color(0.1, 0.2, 0.3, 0.4);
		CHECK(color.r == 0.1f);
		CHECK(color.g == 0.2f);
		CHECK(color.b == 0.3f);
		CHECK(color.a == 0.4f);
	}

	SUBCASE("Int")
	{
		birb::color color(51, 51, 51);
		CHECK(birb::round(color.r, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.g, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.b, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.a, 2) == birb::round(1.0f, 2));
	}

	SUBCASE("Hex")
	{
		birb::color color(0x333333);
		CHECK(birb::round(color.r, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.g, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.b, 2) == birb::round(0.2f, 2));
		CHECK(birb::round(color.a, 2) == birb::round(1.0f, 2));

		birb::color color2(0x51a56a);
		CHECK(birb::round(color2.r, 2) == birb::round(0.32f, 2));
		CHECK(birb::round(color2.g, 2) == birb::round(0.65f, 2));
		CHECK(birb::round(color2.b, 2) == birb::round(0.42f, 2));
		CHECK(birb::round(color2.a, 2) == birb::round(1.0f, 2));
	}
}

TEST_CASE("Color comparison operator")
{
	birb::color color_a(0, 1, 1, 1);
	birb::color color_b(1, 1, 1, 0);
	birb::color color_c(1, 1, 0, 1);
	birb::color color_d(1, 0, 1, 1);

	CHECK(color_a == color_a);
	CHECK_FALSE(color_a == color_b);
	CHECK_FALSE(color_a == color_c);
	CHECK_FALSE(color_a == color_d);
}
