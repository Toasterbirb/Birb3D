#pragma once

#include "Shader.hpp"
#include <glad/gl.h>

namespace birb
{
	class texture
	{
	public:
		texture(const char* image_path, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
		~texture();

		unsigned int id;
		GLenum type;

		void tex_unit(birb::shader& shader, const char* uniform, unsigned int unit);
		void bind();
		void unbind();

	private:

	};
}
