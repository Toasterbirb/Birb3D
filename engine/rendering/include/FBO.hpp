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

		void reload_frame_buffer_texture(const vec2<i32>& dimensions);
		texture& frame_buffer_texture();

	private:
		texture frame_buffer;

		void attach_texture(const texture& texture);
		void setup_rbo(vec2<i32> dimensions);

		std::unique_ptr<rbo> render_buffer_object;
	};
}
