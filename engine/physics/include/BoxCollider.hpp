#pragma once

#include "Vector.hpp"

namespace birb
{
	namespace collider
	{
		class box
		{
		public:
			box();
			~box() = default;
			box(const box&) = default;
			box(box&) = default;

			bool collides_with(const box& box);

			void set_position(const vec3<float>& position);
			void set_size(const vec3<float>& size);
			void set_position_and_size(const vec3<float>& position, const vec3<float>& size);

			// Bounding box values used for collision math
			vec3<float> min() const;
			vec3<float> max() const;

		private:
			void update_min_max_values();

			vec3<float> size;
			vec3<float> position;

			// Cached min and max values
			vec3<float> _min;
			vec3<float> _max;
		};
	}
}
