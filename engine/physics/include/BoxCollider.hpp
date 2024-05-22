#pragma once

#include "Vector.hpp"

namespace birb
{
	class transform;

	namespace collider
	{
		class box
		{
		public:
			box();
			explicit box(const transform& transform);
			~box() = default;
			box(const box&) = default;
			box(box&) = default;

			bool collides_with(const box& box) const;

			void set_position(const vec3<f32>& position);
			void set_size(const vec3<f32>& size);
			void set_position_and_size(const vec3<f32>& position, const vec3<f32>& size);
			void set_position_and_size(const transform& transform);

			vec3<f32> size() const;
			vec3<f32> position() const;

			// Bounding box values used for collision math
			vec3<f32> min() const;
			vec3<f32> max() const;

			/**
			 * @brief Triggers are ignored during collision resolution
			 */
			bool is_trigger = false;

		private:
			void update_min_max_values();

			vec3<f32> _size;
			vec3<f32> _position;

			// Cached min and max values
			vec3<f32> _min;
			vec3<f32> _max;
		};
	}
}
