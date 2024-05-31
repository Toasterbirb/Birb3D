#include "Rectangle.hpp"

namespace birb
{
	namespace ui
	{
		rectangle::rectangle(const vec3<f32> position, const vec2<f32> dimensions, const birb::color color)
		:type(ui_primitive::rectangle), _color(color.to_vec4())
		{
			update_vertex_data(position, dimensions);
		}

		std::vector<f32> rectangle::vertex_data() const
		{
			return vertices;
		}

		glm::vec4 rectangle::color() const
		{
			return _color;
		}

		void rectangle::update_vertex_data(const vec3<f32> pos, const vec2<f32> dim)
		{
			// Create a quad with two triangles
			this->vertices = {
				pos.x,			pos.y + dim.y,	pos.z,
				pos.x,			pos.y,			pos.z,
				pos.x + dim.x, 	pos.y,			pos.z,

				pos.x,			pos.y + dim.y,	pos.z,
				pos.x + dim.x,	pos.y,	 		pos.z,
				pos.x + dim.x,	pos.y + dim.y,	pos.z,
			};

			// 6 vec3 positions      : 6 * 3 = 18
			ensure(vertices.size() == 18);
		}
	}
}
