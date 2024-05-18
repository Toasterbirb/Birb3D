#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "ShaderRef.hpp"
#include "Types.hpp"
#include "Vector.hpp"

namespace birb
{
	struct text
	{
		text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale);
		text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color);
		text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader);
		~text() = default;
		text(const text&) = default;
		text(text&) = default;

		std::string txt;
		birb::font font;
		vec3<f32> position;
		f32 scale;
		birb::color color;
		shader_ref shader;
	};
}
