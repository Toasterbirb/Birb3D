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
		vbo();
		explicit vbo(const std::vector<f32>& data, const bool static_draw = true);

		template<size_t N>
		explicit vbo(const std::array<f32, N>& data, const bool static_draw = true)
		{
			static_assert(N != 0, "Empty data array");

			allocate_buffer();
			set_data(data.data(), data.size(), static_draw);
		}

		~vbo();

		vbo(vbo&) = delete;
		vbo(const vbo&) = delete;
		vbo(vbo&& other);
		vbo& operator=(vbo&& other) noexcept;

		// Reference to the vertex buffer object
		u32 id = 0;

		void set_data(const std::vector<f32>& data, const bool static_draw = true) const;
		void set_data(const f32* data, const size_t size, const bool static_draw) const;

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
		void allocate_buffer();
	};
}
