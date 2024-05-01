#include "Line.hpp"
#include "Math.hpp"
#include "VBO.hpp"

namespace birb
{
	line::line(vec3<f32> a, vec3<f32> b)
	:point_a(a), point_b(b)
	{
		vbo = std::make_shared<birb::vbo>(vertices);
		update_verts();
	}

	void line::update_verts()
	{
		// Calculate the position
		transform.position = center(point_a, point_b);

		// Calculate the size of the line
		assert(false == true && "TODO");
	}
}
