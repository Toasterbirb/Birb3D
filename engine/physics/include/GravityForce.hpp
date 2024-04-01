#pragma once

#include "Force.hpp"

namespace birb
{
	namespace physics_forces
	{
		struct gravity : public force
		{
			void update_force(rigidbody& rigidbody) override;
			vec3<float> gravity = { 0.0f, earth_gravity, 0.0f };

			// Constants
			constexpr static float earth_gravity = -9.81f;
		};
	}
}
