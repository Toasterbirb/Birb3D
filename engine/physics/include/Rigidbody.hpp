#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"

namespace birb
{
	class transform;

	class rigidbody : public editor_component
	{
	public:
		explicit rigidbody(const transform& transform);

		~rigidbody() = default;
		rigidbody(const rigidbody&) = default;
		rigidbody(rigidbody&) = default;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

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
		static inline const std::string editor_header_name = "Rigidbody";
		f32 inverse_mass = 1.0f;
		vec3<f32> force_accumulator;
	};
}
