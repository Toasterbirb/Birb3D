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
		texture(const char* image_path, unsigned int slot, color_format format, unsigned short texture_dimension = 2);
		~texture();

		unsigned int id;

		void tex_unit(birb::shader& shader, const char* uniform, unsigned int unit);
		void bind();
		void unbind();

	private:
		GLenum tex_type;
	};
}
