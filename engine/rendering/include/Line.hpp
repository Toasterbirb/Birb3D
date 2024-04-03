#pragma once

#include "Transform.hpp"
#include "VBO.hpp"

#include <array>
#include <memory>

namespace birb
{
	class line
	{
	public:
		line(vec3<float> a, vec3<float> b);
		~line() = default;
		line(const line&) = default;
		line(line&) = default;

	private:
		void update_verts();

		std::array<float, 6> vertices;
		vec3<float> point_a, point_b;
		component::transform transform;

		std::shared_ptr<birb::vbo> vbo;
	};
}
