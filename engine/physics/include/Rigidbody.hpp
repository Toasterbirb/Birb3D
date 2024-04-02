#pragma once

#include "Transform.hpp"
#include "Vector.hpp"

namespace birb
{
	class rigidbody
	{
	public:
		rigidbody(const component::transform& transform);
		~rigidbody() = default;
		rigidbody(const rigidbody&) = default;
		rigidbody(rigidbody&) = default;

		vec3<float> position;
		vec3<float> velocity;
		vec3<float> acceleration;

		float mass() const;
		void set_mass(float mass);
		void set_mass_infinite();

		void add_force(const vec3<float> force);
		vec3<float> current_force() const;

		void update(const float deltatime);

	private:
		float inverse_mass = 1.0f;
		vec3<float> force_accumulator;
	};
}
