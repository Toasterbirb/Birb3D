#pragma once

#include "Texture.hpp"
#include "Vector.hpp"

#include <memory>

namespace birb
{
	class rbo;

	class fbo
	{
	public:
		explicit fbo(const birb::vec2<i32>& dimensions);
		fbo(const fbo&) = delete;
		~fbo();

		// Reference to the frame buffer object
		u32 id;

		void bind();
		void unbind();

		/**
		 * @brief Bind the frame buffer texture
		 */
		void bind_frame_buffer();

		/**
		 * @brief Unbind the frame buffer texture
		 */
		void unbind_frame_buffer();

		/**
		 * @brief Get the texture ID for the frame buffer texture
		 *
		 * The ID should not be freed in any circumstances
		 */
		u32 frame_buffer_id() const;

		/**
		 * @brief Clear the FBO
		 *
		 * If the FBO has been bound already, it'll be left in bound state
		 */
		void clear();

		void reload_frame_buffer_texture(const vec2<i32>& dimensions);
		// texture& frame_buffer_texture();

	private:
		texture frame_buffer;
		static inline u32 currently_bound_fbo = 0;

		void attach_texture(const texture& texture);
		void setup_rbo(vec2<i32> dimensions);

		std::unique_ptr<rbo> render_buffer_object;
	};
}
