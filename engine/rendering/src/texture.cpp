#include "Texture.hpp"

#include <stb_image.h>
#include <microprofile.h>

#define PROFILER_GROUP "Texture"
#define PROFILER_COLOR MP_RED1

namespace birb
{
	texture::texture(const char* image_path, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type)
	:type(tex_type)
	{
		MICROPROFILE_SCOPEI(PROFILER_GROUP, "Texture loading", PROFILER_COLOR);
		birb::vec2<int> texture_size;
		stbi_set_flip_vertically_on_load(true);
		int texture_color_channel_count;
		unsigned char* tex_bytes = stbi_load(image_path, &texture_size.x, &texture_size.y, &texture_color_channel_count, 0);

		unsigned int texture;
		glGenTextures(1, &id);
		glActiveTexture(slot);
		glBindTexture(tex_type, id);

		glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(tex_type, 0, GL_RGB, texture_size.x, texture_size.y, 0, format, pixel_type, tex_bytes);
		glGenerateMipmap(tex_type);

		stbi_image_free(tex_bytes);
		glBindTexture(tex_type, 0);
	}

	void texture::tex_unit(birb::shader shader, const char* uniform, unsigned int unit)
	{
		unsigned int tex0_uni = glGetUniformLocation(shader.id, uniform);
		shader.activate();
		glUniform1i(tex0_uni, unit);
	}

	void texture::bind()
	{
		glBindTexture(type, id);
	}

	void texture::unbind()
	{
		glBindTexture(type, 0);
	}

	void texture::unload()
	{
		glDeleteTextures(1, &id);
	}
}
