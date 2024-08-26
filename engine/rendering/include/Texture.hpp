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
		texture(const char* image_path, const u32 slot, const color_format format, const texture_type type = texture_type::TEX_2D);
		~texture();
		texture(texture&) = delete;
		texture(const texture&) = delete;
		texture(texture&&) = default; // TODO: Make sure this is okay

		u32 id = 0;

		/**
		 * @brief Create an empty OpenGL texture
		 *
		 * This is can be used for framebuffer stuff etc.
		 * It only initializes the texture and doesn't do any fancy things like
		 * mipmapping and so on
		 */
		void create_empty(const vec2<i32> dimensions, const color_format format, const u8 slot = 0);

		void load(const char* image_path, const u32 slot, const color_format format, const texture_type type = texture_type::TEX_2D);

		void tex_unit(birb::shader& shader, const char* uniform = "tex0", const u32 unit = 0);

		/**
		 * @brief Bind the texture
		 */
		void bind();

		/**
		 * @brief Bind the 2D texture that the id points to
		 */
		static void bind(const u32 texture_id);

		/**
		 * @brief Unbind the 2D texture that is bound currently
		 */
		static void unbind();

		/**
		 * @brief Get the width and height of the texture
		 */
		vec2<i32> size() const;

		f32 aspect_ratio() const;
		f32 aspect_ratio_reverse() const;

		static u32 texture_from_file(const std::string& path);

	private:
		texture_type type = texture_type::TEX_2D;
		u32 slot = 0;

		vec2<i32> dimensions; // Width and height of the texture

		f32 _aspect_ratio = 1.0f;
		f32 _aspect_ratio_reverse = 1.0f;
	};
}
