#pragma once

#include "Shader.hpp"
#include "Vector.hpp"

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
		texture(const texture&) = delete;
		texture(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension = 2);
		~texture();

		unsigned int id = 0;

		/**
		 * @brief Create an empty OpenGL texture
		 *
		 * This is can be used for framebuffer stuff etc.
		 * It only initializes the texture and doesn't do any fancy things like
		 * mipmapping and so on
		 */
		void create_empty(birb::vec2<int> dimensions);

		void load(const char* image_path, const unsigned int slot, const color_format format, const unsigned short texture_dimension = 2);

		void tex_unit(birb::shader& shader, const char* uniform = "tex0", const unsigned int unit = 0);
		void bind();
		void unbind();

		static unsigned int texture_from_file(const std::string& path);

	private:
		GLenum tex_type = GL_TEXTURE_2D;
		unsigned int slot = 0;
	};
}
