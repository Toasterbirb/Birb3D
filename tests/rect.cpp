#include "Rect.hpp"

#include <doctest/doctest.h>

TEST_CASE("Rect construction")
{
	SUBCASE("Default")
	{
		birb::rect<int> rect_int;
		CHECK(rect_int.x == 0);
		CHECK(rect_int.y == 0);
		CHECK(rect_int.w == 0);
		CHECK(rect_int.h == 0);

		birb::rect<float> rect_float;
		CHECK(rect_float.x == 0.0f);
		CHECK(rect_float.y == 0.0f);
		CHECK(rect_float.w == 0.0f);
		CHECK(rect_float.h == 0.0f);
	}

	SUBCASE("Specify values")
	{
		birb::rect<int> rect_int(1, 2, 3, 4);;
		CHECK(rect_int.x == 1);
		CHECK(rect_int.y == 2);
		CHECK(rect_int.w == 3);
		CHECK(rect_int.h == 4);

		birb::rect<float> rect_float (1.11f, 2.22f, 3.33f, 4.44f);
		CHECK(rect_float.x == 1.11f);
		CHECK(rect_float.y == 2.22f);
		CHECK(rect_float.w == 3.33f);
		CHECK(rect_float.h == 4.44f);
	}

	SUBCASE("Array")
	{
		int int_values[4]		= { 1, 2, 3, 4 };
		float float_values[4]	= { 1.11f, 2.22f, 3.33f, 4.44f };

		birb::rect<int> int_rect(int_values);
		CHECK(int_rect.x == 1);
		CHECK(int_rect.y == 2);
		CHECK(int_rect.w == 3);
		CHECK(int_rect.h == 4);

		birb::rect<float> float_rect(float_values);
		CHECK(float_rect.x == 1.11f);
		CHECK(float_rect.y == 2.22f);
		CHECK(float_rect.w == 3.33f);
		CHECK(float_rect.h == 4.44f);
	}
}

TEST_CASE("Rect conversion")
{
	SUBCASE("Int to float")
	{
		constexpr int x = 1, y = 2, w = 3, h = 4;

		birb::rect<int> rect_int(x, y, w, h);
		CHECK(rect_int.to_float() == birb::rect<float>(x, y, w, h));
	}

	SUBCASE("Float to int")
	{
		constexpr float x = 1.11f, y = 2.22f, w = 3.33f, h = 4.44f;

		birb::rect<float> rect_float(x, y, w, h);
		CHECK(rect_float.to_int() == birb::rect<int>(x, y, w, h));
	}
}

TEST_CASE("Calculate the centerpoint of a rect")
{
	CHECK(birb::rect(0, 0, 7, 4).center_point() == birb::vec2<float>(3.5, 2));
	CHECK(birb::rect(2, 1, 13, 7).center_point() == birb::vec2<float>(8.5, 4.5));
	CHECK(birb::rect(1, -2, 5, 7).center_point() == birb::vec2<float>(3.5, 1.5));
}
