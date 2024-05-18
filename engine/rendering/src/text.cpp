#include "Text.hpp"

#include <string>

namespace birb
{
	const std::string default_vert_shader = "text";
	const std::string default_frag_shader = "text";

	text::text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale)
	:txt(txt), font(font), position(position), scale(scale), color(0xFFFFFF), shader(default_vert_shader, default_frag_shader)
	{}

	text::text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color)
	:txt(txt), font(font), position(position), scale(scale), color(color), shader(default_vert_shader, default_frag_shader)
	{}

	text::text(const std::string& txt, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader)
	:txt(txt), font(font), position(position), scale(scale), color(color), shader(shader)
	{}
}
