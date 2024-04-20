#include "Rigidbody.hpp"

#include <cmath>

namespace birb
{
	rigidbody::rigidbody(const component::transform& transform)
	:position(transform.position)
	{}

	f32 rigidbody::mass() const
	{
		if (inverse_mass == 0)
			return INFINITY;

		return 1.0f / inverse_mass;
	}

	void rigidbody::set_mass(f32 mass)
	{
		constexpr f32 lower_limit = 0.0001;

		if (mass < lower_limit)
			inverse_mass = 1.0f / lower_limit;
		else
			inverse_mass = 1.0f / mass;
	}

	void rigidbody::set_mass_infinite()
	{
		inverse_mass = 0.0f;
	}

	void rigidbody::add_force(const vec3<f32> force)
	{
		force_accumulator += force;
	}

	void rigidbody::update(const f32 deltatime)
	{
		// F = ma --> a = F / m (mass is inversed)
		acceleration = force_accumulator * inverse_mass;

		// Reset the force accumulator
		force_accumulator = { 0, 0, 0 };

		// v = v0 + at
		velocity = velocity + acceleration * deltatime;

		// s = s0 + vt (+ 0.5at^2 is ignored for performance reasons)
		position = position + velocity * deltatime;
	}

	vec3<f32> rigidbody::current_force() const
	{
		return force_accumulator;
	}
}
