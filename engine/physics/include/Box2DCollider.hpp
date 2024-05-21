#pragma once

#include "Vector.hpp"

namespace birb
{
	class transform;

	namespace collider
	{
		class box2d
		{
		public:
			box2d();
			explicit box2d(const transform& transform);
			~box2d() = default;
			box2d(const box2d&) = default;
			box2d(box2d&) = default;

			bool collides_with(const box2d& box) const;

			void set_position(const vec2<f32>& position);
			void set_size(const vec2<f32>& size);
			void set_position_and_size(const vec2<f32>& position, const vec2<f32>& size);
			void set_position_and_size(const transform& transform);

			vec2<f32> size() const;
			vec2<f32> position() const;

			// Bounding box values used for collision math
			vec2<f32> min() const;
			vec2<f32> max() const;

		private:
			void update_min_max_values();

			vec2<f32> _size;
			vec2<f32> _position;

			// Cached min and max values
			vec2<f32> _min;
			vec2<f32> _max;
		};
	}
}
