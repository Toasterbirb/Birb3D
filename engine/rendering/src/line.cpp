#include "Line.hpp"
#include "Math.hpp"

namespace birb
{
	line::line(vec3<float> a, vec3<float> b)
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
