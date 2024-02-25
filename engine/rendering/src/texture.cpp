#include "Profiling.hpp"
#include "Texture.hpp"

#include <cassert>
#include <stb_image.h>

namespace birb
{
	texture::texture(const char* image_path, unsigned int slot, color_format format, unsigned short texture_dimension)
	{
		PROFILER_SCOPE_IO("Texture loading")

		assert(image_path != nullptr && "Null image path");

		birb::vec2<int> texture_size;
		stbi_set_flip_vertically_on_load(true);
		int texture_color_channel_count;

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

		unsigned char* tex_bytes = stbi_load(image_path, &texture_size.x, &texture_size.y, &texture_color_channel_count, 0);
		if (tex_bytes == nullptr)
		{
			birb::log_error("Can't open a texture at: " + std::string(image_path));
			return;
		}

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(tex_type, id);

		glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(tex_type, 0, GL_RGB, texture_size.x, texture_size.y, 0, gl_color_format, GL_UNSIGNED_BYTE, tex_bytes);
		glGenerateMipmap(tex_type);

		stbi_image_free(tex_bytes);
		glBindTexture(tex_type, 0);
	}

	texture::~texture()
	{
		glDeleteTextures(1, &id);
	}

	void texture::tex_unit(birb::shader& shader, const char* uniform, unsigned int unit)
	{
		shader.add_uniform_location(uniform);
		shader.set_var(uniform, unit);
	}

	void texture::bind()
	{
		glBindTexture(tex_type, id);
	}

	void texture::unbind()
	{
		glBindTexture(tex_type, 0);
	}
}
