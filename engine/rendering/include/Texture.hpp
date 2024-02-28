#pragma once

#include "Shader.hpp"
#include <glad/gl.h>

namespace birb
{
	enum class color_format
	{
		RGB, RGBA
	};

	class texture
	{
	public:
		texture(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension = 2);
		~texture();

		unsigned int id;

		void tex_unit(birb::shader& shader, const char* uniform = "tex0", const unsigned int unit = 0);
		void bind();
		void unbind();

	private:
		GLenum tex_type;
		unsigned int slot;
	};
}
