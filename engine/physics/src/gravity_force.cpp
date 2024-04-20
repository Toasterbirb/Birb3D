#include "GravityForce.hpp"
#include "Vector.hpp"

namespace birb
{
	namespace physics_forces
	{
		void gravity::update_force(rigidbody& rigidbody)
		{
			// G = mg
			vec3<f32> G = gravity * rigidbody.mass();

			rigidbody.add_force(G);
		}
	}
}
