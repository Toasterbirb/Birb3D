#pragma once

#include "Transform.hpp"

#include <array>
#include <memory>

namespace birb
{
	class vbo;

	class line
	{
	public:
		line(vec3<f32> a, vec3<f32> b);
		~line() = default;
		line(const line&) = default;
		line(line&) = default;

	private:
		void update_verts();

		std::array<f32, 6> vertices;
		vec3<f32> point_a, point_b;
		birb::transform transform;

		std::shared_ptr<birb::vbo> vbo;
	};
}
