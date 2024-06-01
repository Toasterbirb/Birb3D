#pragma once

#include "Color.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <glm/glm.hpp>
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
			virtual glm::vec4 color() const = 0;
		};
	}
}
