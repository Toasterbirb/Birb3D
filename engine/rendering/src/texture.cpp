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

	unsigned int texture::texture_from_file(const std::string& path)
	{
		assert(!path.empty());

		// Load the image data
		asset::image image(path.c_str(), true);
		assert(image.data != nullptr);
		assert(image.dimensions.x != 0);
		assert(image.dimensions.y != 0);
		assert(image.color_channels != 0);

		unsigned int id;

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		FIXME("Handle RGB textures that don't have an alpha channel");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.dimensions.x, image.dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return id;
	}
}
