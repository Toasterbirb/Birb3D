#include "Rigidbody.hpp"
#include "Transform.hpp"

#include <cmath>
#include <imgui.h>

namespace birb
{
	rigidbody::rigidbody(const transform& transform)
	:position(transform.position)
	{}

	void rigidbody::draw_editor_ui()
	{
		ImGui::Text("Position:     [%.2f, %.2f, %.2f]", position.x, position.y, position.z);
		ImGui::Text("Velocity:     [%.2f, %.2f, %.2f]", velocity.x, velocity.y, velocity.z);
		ImGui::Text("Acceleration: [%.2f, %.2f, %.2f]", acceleration.x, acceleration.y, acceleration.z);
		ImGui::Spacing();

		static f32 new_mass = 1.0 / inverse_mass;

		ImGui::PushItemWidth(ImGui::CalcItemWidth() / 2);
		ImGui::InputFloat("Mass", &new_mass);
		ImGui::SameLine();
		if (ImGui::Button("Update"))
			set_mass(new_mass);
		ImGui::PopItemWidth();
	}

	std::string rigidbody::collapsing_header_name() const
	{
		return editor_header_name;
	}

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
