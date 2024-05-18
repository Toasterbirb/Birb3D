#include "Assert.hpp"
#include "Character.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/gl.h>
#include <map>

u8 counter = 0;

namespace birb
{
	font_manager::font_manager()
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

		for (u8 i = 0; i < glyph_count; ++i)
		{
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
		}

		// Free the font face
		FT_Done_Face(font_face);

		return font(character_map, size);
	}
}
