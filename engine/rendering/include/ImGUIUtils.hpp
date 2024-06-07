#pragma once

#include "Types.hpp"
#include "Vector.hpp"

#include <string>

namespace birb
{
	namespace imgui_utils
	{
		void draw_texture(const std::string& window_name, const u32 texture_id, const vec2<i32> texture_dimensions);
	}
}
