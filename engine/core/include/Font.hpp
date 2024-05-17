#pragma once

#include <map>
#include <memory>

namespace birb
{
	struct character;

	class font
	{
	public:
		explicit font(const std::shared_ptr<std::map<char, character>> character_map);
		~font() = default;
		font(const font&) = default;
		font(font&) = default;

	private:
		std::shared_ptr<std::map<char, character>> character_map;
	};
}
