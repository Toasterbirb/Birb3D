#pragma once

#include "Color.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <vector>

namespace birb
{
	namespace ui
	{
		class ui_base
		{
		public:
			ui_base();

			virtual std::vector<f32> vertex_data() const = 0;
			/* Format for the vertex data:
			 * vec3 position | vec4 color */

			const u64 uuid;
			vec3<f32> position;
			birb::color color;

		private:

		};
	}
}
