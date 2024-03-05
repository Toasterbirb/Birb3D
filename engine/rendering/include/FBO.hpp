#pragma once

#include "Texture.hpp"
#include "Vector.hpp"

namespace birb
{
	class fbo
	{
	public:
		explicit fbo(const birb::vec2<int>& dimensions);
		fbo(const fbo&) = delete;
		~fbo();

		// Reference to the frame buffer object
		unsigned int id;

		void bind();
		void unbind();

		void reload_frame_buffer_texture(const vec2<int>& dimensions);
		texture& frame_buffer_texture();

	private:
		texture frame_buffer;

		void attach_texture(const texture& texture);
		void setup_rbo(vec2<int> dimensions);

		unsigned int rbo = 0;
	};
}
