#pragma once

#include "Force.hpp"
#include "Vector.hpp"

#include <numbers>

namespace birb
{
	namespace physics_forces
	{
		struct gravity : public force
		{
			void update_force(rigidbody& rigidbody) override;
			vec3<f32> gravity = { 0.0f, earth_gravity, 0.0f };

			// Constants
			constexpr static f32 earth_gravity = std::numbers::pi * std::numbers::pi * -1;
		};
	}
}
