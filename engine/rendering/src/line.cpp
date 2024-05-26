#include "Line.hpp"

namespace birb
{
	line::line(vec3<f32> a, vec3<f32> b)
	:point_a(a), point_b(b)
	{
		vao = std::make_shared<birb::vao>();

		vao->bind();
		vbo = std::make_shared<birb::vbo>(vertices);
		update_verts();
		vao->link_vbo(*vbo, 0, 3, 3, 0);
		vao->unbind();
	}

	void line::update_verts()
	{
		// Point A
		vertices[0] = point_a.x;
		vertices[1] = point_a.y;
		vertices[2] = point_a.z;

		// Point B
		vertices[3] = point_b.x;
		vertices[4] = point_b.y;
		vertices[5] = point_b.z;

		vbo->bind();
		vbo->set_data(vertices.data(), vertices.size(), false);
		vao->unbind();
		vbo->unbind();
	}
}
