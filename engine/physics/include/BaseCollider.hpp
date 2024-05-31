#pragma once

#include "Types.hpp"
#include "Vector.hpp"

namespace birb
{
	namespace collider
	{
		class box;

		class base_collider
		{
		public:
			virtual bool collides_with(const box& box) const = 0;

			virtual vec3<f32> size() const = 0;
			virtual vec3<f32> position() const = 0;

			/**
			 * @brief Triggers are ignored during collision resolution
			 */
			bool is_trigger = false;

		private:

		};
	}
}
