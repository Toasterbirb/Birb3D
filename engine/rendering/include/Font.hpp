#pragma once

#include "Types.hpp"
#include "Vector.hpp"

#include <map>
#include <memory>

namespace birb
{
	struct character;

	class font
	{
	public:
		explicit font(const std::shared_ptr<std::map<char, character>> character_map, const u8 size, const u64 uuid);
		~font() = default;
		font(const font&) = default;
		font(font&) = default;

		character& get_char(const char c) const;
		vec2<u32> char_dimensions(const char c) const;
		const u8 size;
		const u64 uuid;

	private:
		std::shared_ptr<std::map<char, character>> character_map;
	};
}
