#pragma once

#include "Types.hpp"

#include <array>
#include <vector>

namespace birb
{
	// Vertex buffer object
	class vbo
	{
	public:
		explicit vbo(const std::vector<float>& vertices);

		template<size_t N>
		explicit vbo(const std::array<float, N>& vertices)
		{
			static_assert(N != 0, "Empty vertex array");
			static_assert(N < 33000, "You might wanna check the vert count on that model");

			load(vertices.data(), vertices.size());
		}

		vbo(const vbo&) = delete;
		~vbo();

		// Reference to the vertex buffer object
		u32 id = 0;

		void bind();
		void unbind();

#ifndef NDEBUG
		u32 d_vert_count = 0;
#endif

	private:
		void load(const float* vertices, const size_t size);
	};
}
