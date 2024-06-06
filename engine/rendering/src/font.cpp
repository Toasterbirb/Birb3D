#include "Assert.hpp"
#include "Character.hpp"
#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace birb
{
	font::font(const std::shared_ptr<std::map<char, character>> character_map, const u8 size, const u64 uuid, std::shared_ptr<texture> bitmap)
	:size(size), uuid(uuid), bitmap(bitmap), character_map(character_map)
	{
		ensure(character_map.get(), "Unallocated character map");
		ensure(!character_map->empty(), "Empty character map");
	}

	character& font::get_char(const char c) const
	{
		return character_map->at(c);
	}

	vec2<u32> font::char_dimensions(const char c) const
	{
		return character_map->at(c).size;
	}
}
