#pragma once

#include "Material.hpp"

#include <unordered_map>

namespace birb
{
	namespace parser
	{
		std::unordered_map<std::string, material> mtl(const std::string& mtl_file_path);
	}
}
