#pragma once

#include "GLBuffer.hpp"
#include "Types.hpp"

#include <array>
#include <vector>

namespace birb
{
	// Vertex buffer object
	class vbo
	{
	public:
		vbo();
		explicit vbo(const std::vector<f32>& data, const gl_usage usage = gl_usage::static_draw);

		template<size_t N>
		explicit vbo(const std::array<f32, N>& data, const gl_usage usage = gl_usage::static_draw)
		:buffer(gl_buffer_type::array)
		{
			static_assert(N != 0, "Empty data array");
			bind();
			set_data(data.data(), data.size(), usage);
			unbind();
		}

		~vbo() = default;
		vbo(vbo&) = delete;
		vbo(const vbo&) = delete;
		vbo(vbo&&) = default;

		// Reference to the vertex buffer object
		u32 id() const;

		void set_data(const std::vector<f32>& data, const gl_usage usage = gl_usage::static_draw) const;
		void set_data(const f32* data, const size_t size, const gl_usage usage) const;

		void update_data(const f32* data, const size_t size, const u32 offset = 0) const;

		void enable_vertex_attrib_array(const u32 index) const;
		void disable_vertex_attrib_array(const u32 index) const;

		void set_vertex_attrib_ptr(const u32 layout_index,
				const u32 component_index,
				const size_t component_size,
				const u32 components_per_vert) const;

		void bind() const;

		/**
		 * @brief Unbind the currently bound VBO
		 */
		static void unbind();

	private:
		static inline u32 d_currently_bound_vbo = 0;
		gl_buffer buffer;
	};
}
