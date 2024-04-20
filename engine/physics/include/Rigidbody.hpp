#pragma once

#include "Transform.hpp"
#include "Vector.hpp"

namespace birb
{
	class rigidbody
	{
	public:
		explicit rigidbody(const component::transform& transform);
		~rigidbody() = default;
		rigidbody(const rigidbody&) = default;
		rigidbody(rigidbody&) = default;

		vec3<f32> position;
		vec3<f32> velocity;
		vec3<f32> acceleration;

		f32 mass() const;
		void set_mass(f32 mass);
		void set_mass_infinite();

		void add_force(const vec3<f32> force);
		vec3<f32> current_force() const;

		void update(const f32 deltatime);

	private:
		f32 inverse_mass = 1.0f;
		vec3<f32> force_accumulator;
	};
}
