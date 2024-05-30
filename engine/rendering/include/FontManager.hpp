#pragma once

#include "Types.hpp"
#include "Vector.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

namespace birb
{
	class font;

	class font_manager
	{
	public:
		font_manager();
		~font_manager();
		font_manager(const font_manager&) = delete;
		font_manager(font_manager&) = delete;

		/**
		 * @brief Load a font from file
		 *
		 * @param font_file Relative path to the font file on disk
		 * @param size 		Height of the font in pixels
		 */
		font load_font(const std::string& font_file, const u8 size);

	private:
		FT_Library ft;
	};
}
