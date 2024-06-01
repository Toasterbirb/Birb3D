#pragma once

#include "Color.hpp"
#include "Types.hpp"
#include "UIBase.hpp"
#include "UIPrimitive.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace birb
{
	namespace ui
	{
		class rectangle : public ui_base
		{
		public:
			rectangle(const vec3<f32> position, const vec2<f32> dimensions, const birb::color color);
			~rectangle() = default;
			rectangle(const rectangle&) = default;
			rectangle(rectangle&) = default;

			glm::vec4 color() const override;
			std::vector<f32> vertex_data() const override;

			const ui_primitive type;

		protected:
			std::vector<f32> vertices;
			glm::vec4 _color;

			/**
			 * @brief Update the vertices array
			 *
			 * @param pos position
			 * @param dim dimensions
			 * @param col color
			 */
			void update_vertex_data(const vec3<f32> pos, const vec2<f32> dim);
		};
	}
}
