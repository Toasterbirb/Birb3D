#include "Transform.hpp"
#include "Vector.hpp"

#include <doctest/doctest.h>

TEST_CASE("birb::transform")
{
	birb::transform transform;

	SUBCASE("Default values")
	{
		CHECK(transform.position == birb::vec3<f32>(0.0f, 0.0f, 0.0f));
		CHECK(transform.rotation == birb::vec3<f32>(0.0f, 0.0f, 0.0f));
		CHECK(transform.local_scale == birb::vec3<f32>(1.0f, 1.0f, 1.0f));
	}

	SUBCASE("Translate")
	{
		transform.translate(1.0f, -2.0f, 0.0f);
		CHECK(transform.position == birb::vec3<f32>(1.0f, -2.0f, 0.0f));

		transform.translate(0.0f, -2.0f, 0.0f);
		CHECK(transform.position == birb::vec3<f32>(1.0f, -4.0f, 0.0f));
	}

	SUBCASE("Rotate")
	{
		transform.rotate(10.0f, 20.0f, -30.0f);
		CHECK(transform.rotation == birb::vec3<f32>(10.0f, 20.0f, -30.0f));

		transform.rotate(0.0f, -20.0f, 0.0f);
		CHECK(transform.rotation == birb::vec3<f32>(10.0f, 0.0f, -30.0f));
	}

	SUBCASE("Scale")
	{
		transform.scale(2.0f, 1.0f, 0.0f);
		CHECK(transform.local_scale == birb::vec3<f32>(2.0f, 1.0f, 0.0f));

		transform.scale(4.0f, 1.0f, 0.0f);
		CHECK(transform.local_scale == birb::vec3<f32>(8.0f, 1.0f, 0.0f));
	}
}
