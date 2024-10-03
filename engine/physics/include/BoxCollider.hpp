#pragma once

#include "BaseCollider.hpp"
#include "EditorComponent.hpp"
#include "Vector.hpp"

namespace birb
{
	class transform;

	namespace collider
	{
		class box : public base_collider, editor_component
		{
		public:
			box();
			explicit box(const transform& transform);
			~box() = default;
			box(const box&) = default;
			box(box&) = default;

			void draw_editor_ui() override;
			std::string collapsing_header_name() const override;

			bool collides_with(const box& box) const override;

			void set_position(const vec3<f32>& position);
			void set_size(const vec3<f32>& size);
			void set_position_and_size(const vec3<f32>& position, const vec3<f32>& size);
			void set_position_and_size(const transform& transform);

			vec3<f32> size() const override;
			vec3<f32> position() const override;

			// Bounding box values used for collision math
			vec3<f32> min() const;
			vec3<f32> max() const;

		private:
			static inline const std::string editor_header_name = "Box collider";
			void update_min_max_values();

			vec3<f32> _size;
			vec3<f32> _position;

			// Cached min and max values
			vec3<f32> _min;
			vec3<f32> _max;
		};
	}
}
