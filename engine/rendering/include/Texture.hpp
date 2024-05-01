#pragma once

#include "ColorFormat.hpp"
#include "Shader.hpp"
#include "Vector.hpp"

namespace birb
{
	enum class texture_type
	{
		TEX_1D = 3552,
		TEX_2D = 3553,
		TEX_3D = 32879,
	};

	class texture
	{
	public:
		texture() = default;
		texture(const texture&) = delete;
		texture(const char* image_path, const u32 slot, const color_format format, const u16 texture_dimension = 2);
		~texture();

		u32 id = 0;

		/**
		 * @brief Create an empty OpenGL texture
		 *
		 * This is can be used for framebuffer stuff etc.
		 * It only initializes the texture and doesn't do any fancy things like
		 * mipmapping and so on
		 */
		void create_empty(birb::vec2<i32> dimensions);

		void load(const char* image_path, const u32 slot, const color_format format, const u16 texture_dimension = 2);

		void tex_unit(birb::shader& shader, const char* uniform = "tex0", const u32 unit = 0);
		void bind();
		void unbind();

		/**
		 * @brief Get the width and height of the texture
		 */
		vec2<i32> size();

		f32 aspect_ratio() const;
		f32 aspect_ratio_reverse() const;

		static u32 texture_from_file(const std::string& path);

	private:
		i32 tex_type = static_cast<i32>(texture_type::TEX_2D);
		u32 slot = 0;

		vec2<i32> dimensions; // Width and height of the texture

		f32 _aspect_ratio = 1.0f;
		f32 _aspect_ratio_reverse = 1.0f;
	};
}
