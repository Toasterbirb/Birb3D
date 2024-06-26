#pragma once

#include "Color.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "Vector.hpp"

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

		birb::color color;
		std::shared_ptr<birb::vao> vao;

	private:
		void update_verts();

		std::array<f32, 6> vertices;
		vec3<f32> point_a, point_b;

		std::shared_ptr<birb::vbo> vbo;
	};
}
