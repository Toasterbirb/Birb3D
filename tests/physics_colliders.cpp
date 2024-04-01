#include <doctest/doctest.h>

#include "BoxCollider.hpp"

TEST_CASE("3D box collider")
{
	{
		birb::collider::box box_a;
		box_a.set_position({ 0, 0, 0 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, 3, 0 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 1 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 1, 0, -2 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 0 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, 1.01, 0 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 0 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, -3.01, 0 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 0 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, -1, 2.01 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 0 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, -1, -2.01 });
		box_b.set_size({ 1, 1, 1 });

		CHECK_FALSE(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 1 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 1, 0, 0 });
		box_b.set_size({ 1, 1, 1 });

		CHECK(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 1 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ -1, 0, 2 });
		box_b.set_size({ 1, 1, 1 });

		CHECK(box_a.collides_with(box_b));
	}

	{
		birb::collider::box box_a;
		box_a.set_position({ 0, -1, 1 });
		box_a.set_size({ 1, 1, 1 });

		birb::collider::box box_b;
		box_b.set_position({ 0, 0, 0 });
		box_b.set_size({ 1, 1, 1 });

		CHECK(box_a.collides_with(box_b));
	}
}
