#include "Assert.hpp"
#include "GLSupervisor.hpp"
#include "Globals.hpp"
#include "Image.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Texture.hpp"
#include "Vector.hpp"

#include <glad/gl.h>
#include <stb_image.h>
#include <string>

static_assert(GL_TEXTURE_1D == 3552);
static_assert(GL_TEXTURE_2D == 3553);
static_assert(GL_TEXTURE_3D == 32879);

namespace birb
{
	texture::texture(const char* image_path, const u32 slot, const color_format format, const u16 texture_dimension)
	:slot(slot)
	{
		this->load(image_path, slot, format, texture_dimension);
	}

	void texture::create_empty(const vec2<i32> dimensions, const color_format format, const u8 slot)
	{
		ensure(id == 0, "Memory leak");
		ensure(dimensions.x > 0);
		ensure(dimensions.y > 0);

		this->dimensions = dimensions;
		this->slot = slot;

		glGenTextures(1, &id);
		ensure(id != 0);

		glBindTexture(GL_TEXTURE_2D, id);

		// Use float for the depth map
		if (format == color_format::DEPTH)
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<i32>(format), dimensions.x, dimensions.y, 0, static_cast<i32>(format), GL_FLOAT, NULL);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<i32>(format), dimensions.x, dimensions.y, 0, static_cast<i32>(format), GL_UNSIGNED_INT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Enable texture wrapping for depth maps
		if (format == color_format::DEPTH)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	void texture::load(const char* image_path, const u32 slot, const color_format format, const u16 texture_dimension)
	{
		PROFILER_SCOPE_IO("Texture loading");

		ensure(id == 0, "Memory leak");

		this->slot = slot;

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
		ensure(tex_type != 0, "Invalid texture type");

		birb::asset::image texture(image_path, true);
		this->dimensions = texture.dimensions;

		// Calculate the aspect ratio from the texture size
		_aspect_ratio = static_cast<f32>(dimensions.x) / static_cast<f32>(dimensions.y);
		_aspect_ratio_reverse = static_cast<f32>(dimensions.y) / static_cast<f32>(dimensions.x);

		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(tex_type, id);

		glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(tex_type, 0, static_cast<i32>(format), texture.dimensions.x, texture.dimensions.y, 0, static_cast<i32>(format), GL_UNSIGNED_BYTE, texture.data);
		glGenerateMipmap(tex_type);

		glBindTexture(tex_type, 0);

		birb::log("Texture loaded [", image_path, "] (", ptr_to_str(this), ")");
	}

	texture::~texture()
	{
		ensure(id != 0, "Attempted to destruct a texture that wasn't initialized");
		ensure(birb::g_opengl_initialized);

		birb::log("Texture destroyed (" + ptr_to_str(this) + ")");

		glDeleteTextures(1, &id);
		id = 0;
	}

	void texture::tex_unit(birb::shader& shader, const char* uniform, const u32 unit)
	{
		ensure(id != 0, "Texture needs to be initialized at this point");

		shader.set_int(uniform, unit);
	}

	void texture::bind()
	{
		ensure(id != 0, "Texture needs to be initialized at this point");
		gl_supervisor gls;

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(tex_type, id);
	}

	void texture::bind(const u32 texture_id)
	{
		ensure(texture_id != 0, "Use the unbind() function if you need to unbind a texture");
		gl_supervisor gls;

		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	void texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	u32 texture::texture_from_file(const std::string& path)
	{
		PROFILER_SCOPE_IO_FN();

		ensure(!path.empty());

		// Load the image data
		asset::image image(path.c_str(), true);
		ensure(image.data != nullptr);
		ensure(image.dimensions.x != 0);
		ensure(image.dimensions.y != 0);
		ensure(image.color_channels != 0);

		u32 id;

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

	vec2<i32> texture::size() const
	{
		return dimensions;
	}

	f32 texture::aspect_ratio() const
	{
		return _aspect_ratio;
	}

	f32 texture::aspect_ratio_reverse() const
	{
		return _aspect_ratio_reverse;
	}
}
