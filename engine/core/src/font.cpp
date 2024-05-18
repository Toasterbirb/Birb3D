#include "Assert.hpp"
#include "Character.hpp"
#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace birb
{
	font::font(const std::shared_ptr<std::map<char, character>> character_map, const u8 size)
	:character_map(character_map), _size(size)
	{
		ensure(character_map.get(), "Unallocated character map");
		ensure(!character_map->empty(), "Empty character map");
	}

	character& font::get_char(const char c) const
	{
		return character_map->at(c);
	}

	u8 font::size() const
	{
		return _size;
	}
}
