#include "Profiling.hpp"
#include "Texture.hpp"

#include <stb_image.h>

namespace birb
{
	texture::texture(const char* image_path, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type)
	:type(tex_type)
	{
		PROFILER_SCOPE_IO("Texture loading")
		birb::vec2<int> texture_size;
		stbi_set_flip_vertically_on_load(true);
		int texture_color_channel_count;

		unsigned char* tex_bytes = stbi_load(image_path, &texture_size.x, &texture_size.y, &texture_color_channel_count, 0);
		if (tex_bytes == nullptr)
		{
			birb::log_error("Can't open a texture at: " + std::string(image_path));
			return;
		}

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
