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
		texture() = default;
		texture(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension = 2);
		~texture();

		unsigned int id = 0;

		void load(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension = 2);

		void tex_unit(birb::shader& shader, const char* uniform = "tex0", const unsigned int unit = 0);
		void bind();
		void unbind();

		static unsigned int texture_from_file(const std::string& path);

	private:
		GLenum tex_type;
		unsigned int slot = 0;
	};
}
