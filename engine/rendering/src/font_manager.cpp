#include "Assert.hpp"
#include "Character.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "GLSupervisor.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "UUID.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <map>

static u8 counter = 0;

namespace birb
{
	font_manager::font_manager()
	:glyph_shader_ref("text_bitmap"), bitmap_fbo(vec2<i32>(bitmap_atlas_size, bitmap_atlas_size), color_format::RGBA)
	{
		ensure(counter == 0, "There should be only a singular font_manager at any time");
		counter++;

		if (FT_Init_FreeType(&ft))
			log_fatal(1, "Failed to init FreeType");
	}

	font_manager::~font_manager()
	{
		ensure(counter == 1, "Multiple font_manager were created at some point");
		counter--;
		FT_Done_FreeType(ft);
	}

	font font_manager::load_font(const std::string& font_file, const u8 size)
	{
		PROFILER_SCOPE_MISC_FN();
		log("Loading font: ", font_file, " (size: ", static_cast<unsigned int>(size), ")");

		FT_Face font_face;
		if (FT_New_Face(ft, font_file.c_str(), 0, &font_face))
			log_fatal(2, "Failed to load font: ", font_file);

		FT_Set_Pixel_Sizes(font_face, 0, size);

		std::shared_ptr<std::map<char, character>> character_map = std::make_shared<std::map<char, character>>();

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Allocate memory for all of the OpenGL textures at once
		constexpr u8 glyph_count = 128;
		std::array<u32, glyph_count> glyph_texture_ids;
		glGenTextures(glyph_count, glyph_texture_ids.data());

		// Start rendering the glyphs into the bitmap fbo
		constexpr u8 characters_per_row = 16;
		const glm::vec2 glyph_atlas_dimensions(size, size);
		glm::vec2 glyph_atlas_position(0.0f, 0.0f);
		glm::mat4 orthographic_projection = glm::ortho(0.0f, bitmap_atlas_size, 0.0f, bitmap_atlas_size);

		vao glyph_vao;
		vbo glyph_vbo;

		glyph_vao.bind();
		glyph_vbo.bind();

		const f32 glyph_quad_verts[6][4] = {
			{ 0,							glyph_atlas_dimensions.y,	0.0f, 0.0f },
			{ 0,							0,							0.0f, 1.0f },
			{ glyph_atlas_dimensions.x, 	0,							1.0f, 1.0f },

			{ 0,							glyph_atlas_dimensions.y,	0.0f, 0.0f },
			{ glyph_atlas_dimensions.x,		0, 							1.0f, 1.0f },
			{ glyph_atlas_dimensions.x,		glyph_atlas_dimensions.y,	1.0f, 0.0f }
		};
		glyph_vbo.set_data(*glyph_quad_verts, sizeof(glyph_quad_verts), true);
		glyph_vao.link_vbo(glyph_vbo, 0, 4, 4, 0);

		glyph_vao.unbind();
		glyph_vbo.unbind();

		std::shared_ptr<shader> glyph_shader = shader_collection::get_shader(glyph_shader_ref);
		glyph_shader->activate();
		glyph_shader->set(shader_uniforms::glyph, 0);
		glyph_shader->set(shader_uniforms::custom_projection, orthographic_projection);
		glActiveTexture(GL_TEXTURE0);

		bitmap_fbo.bind();
		glyph_vao.bind();

		for (u8 i = 0; i < glyph_count; ++i)
		{
			GL_SUPERVISOR_SCOPE();

			// Load the glyph
			if (FT_Load_Char(font_face, i, FT_LOAD_RENDER))
				log_fatal(1, "Failed to load glyph: ", static_cast<char>(i));

			// Generate the texture
			ensure(glyph_texture_ids.at(i) != 0, "Missing OpenGL texture");
			glBindTexture(GL_TEXTURE_2D, glyph_texture_ids[i]);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				font_face->glyph->bitmap.width,
				font_face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				font_face->glyph->bitmap.buffer
			);

			// Texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			character character = {
				glyph_texture_ids[i],
				vec2<u32>(font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows),
				vec2<i32>(font_face->glyph->bitmap_left, font_face->glyph->bitmap_top),
				font_face->glyph->advance.x
			};

			character_map->insert(std::pair<char, birb::character>(i, character));

			// Draw the texture into the bitmap atlas
			// Each character will use up size * size worth of space
			glyph_shader->set(shader_uniforms::glyph_position, glyph_atlas_position);
			glBindTexture(GL_TEXTURE_2D, glyph_texture_ids[i]);
			glyph_vao.bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Draw up-to x amount of characters per row
			if (glyph_atlas_position.x >= glyph_atlas_dimensions.x * characters_per_row)
			{
				glyph_atlas_position.x = 0.0f;
				glyph_atlas_position.y += glyph_atlas_dimensions.y;
			}
		}

		glyph_vao.unbind();
		bitmap_fbo.unbind();

		// Free the font face
		FT_Done_Face(font_face);

		return font(character_map, size, uuid::generate(), std::move(bitmap_fbo.frame_buffer));
	}
}
