#include "Image.hpp"
#include "Profiling.hpp"
#include "Texture.hpp"

#include <string>

#include <cassert>
#include <stb_image.h>

namespace birb
{
	texture::texture(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension)
	:slot(slot)
	{
		this->load(image_path, slot, format, texture_dimension);
	}

	void texture::load(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension)
	{
		PROFILER_SCOPE_IO("Texture loading")

		this->slot = slot;

		GLenum gl_color_format = 0;
		switch (format)
		{
			case birb::color_format::RGB:
				gl_color_format = GL_RGB;
				break;

			case birb::color_format::RGBA:
				gl_color_format = GL_RGBA;
				break;
		}
		assert(gl_color_format != 0 && "GL color format wasn't set correctly");

		tex_type = 0;
		switch (texture_dimension)
		{
			case (1):
				tex_type = GL_TEXTURE_1D;
				break;

			case (2):
				tex_type = GL_TEXTURE_2D;
				break;

			case (3):
				tex_type = GL_TEXTURE_3D;
				break;
		}
		assert(tex_type != 0 && "Invalid texture type");

		birb::asset::image texture(image_path, true);

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(tex_type, id);

		glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(tex_type, 0, GL_RGB, texture.dimensions.x, texture.dimensions.y, 0, gl_color_format, GL_UNSIGNED_BYTE, texture.data);
		glGenerateMipmap(tex_type);

		glBindTexture(tex_type, 0);
	}

	texture::~texture()
	{
		assert(id != 0 && "Attempted to destruct a texture that wasn't initialized");

		glDeleteTextures(1, &id);
	}

	void texture::tex_unit(birb::shader& shader, const char* uniform, const unsigned int unit)
	{
		assert(id != 0 && "Texture needs to be initialized at this point");

		shader.set_int(uniform, unit);
	}

	void texture::bind()
	{
		assert(id != 0 && "Texture needs to be initialized at this point");

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(tex_type, id);
	}

	void texture::unbind()
	{
		assert(id != 0 && "Texture needs to be initialized at this point");

		glBindTexture(tex_type, 0);
	}
}
