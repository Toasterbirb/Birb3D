#pragma once

#include "Types.hpp"
#include "Vector.hpp"

#include <string>

namespace birb
{
	namespace imgui
	{
		void texture(const std::string& window_name, const u32 texture_id, const vec2<i32> texture_dimensions);

		bool vec2i_slider(const std::string& window_name, const std::string& label, vec2<i32>& vec, const i32 min, const i32 max);
		bool vec3i_slider(const std::string& window_name, const std::string& label, vec3<i32>& vec, const i32 min, const i32 max);
		bool vec2f_slider(const std::string& window_name, const std::string& label, vec2<f32>& vec, const f32 min, const f32 max);
		bool vec3f_slider(const std::string& window_name, const std::string& label, vec3<f32>& vec, const f32 min, const f32 max);
	}
}
