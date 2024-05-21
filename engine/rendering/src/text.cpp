#include "Assert.hpp"
#include "Character.hpp"
#include "Profiling.hpp"
#include "Text.hpp"

#include <string>

namespace birb
{
	const std::string default_vert_shader = "text";
	const std::string default_frag_shader = "text";

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position)
	:font(font), position(position), scale(1.0f), color(0xFFFFFF), shader(default_vert_shader, default_frag_shader)
	{
		set_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale)
	:font(font), position(position), scale(scale), color(0xFFFFFF), shader(default_vert_shader, default_frag_shader)
	{
		set_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color)
	:font(font), position(position), scale(scale), color(color), shader(default_vert_shader, default_frag_shader)
	{
		set_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader)
	:font(font), position(position), scale(scale), color(color), shader(shader)
	{
		set_text(text);
	}

	void text::set_text(const std::string& text)
	{
		PROFILER_SCOPE_RENDER_FN();

		this->txt = text;

		if (_chars.get())
			_chars.reset();

		_chars = std::make_shared<std::vector<char_data>>();

		f32 x = position.x;
		f32 y = position.y;

		for (const char c : txt)
		{
			// Process newline characters
			if (c == '\n')
			{
				// Go back to the starting position width wise
				x = position.x;

				// Go one line down
				y -= font.size() * scale;

				// We shouldn't draw the newline char
				continue;
			}

			const character& ch = font.get_char(c);

			// position
			const vec2<f32> pos(
				x + ch.bearing.x * scale,
				y - (ch.size.y - ch.bearing.y) * scale
			);

			char_data c_data;

			// Avoid storing duplicated dimension data
			if (!_char_dimensions.contains(c))
			{
				_char_dimensions[c] = {
					ch.size.x * scale,
					ch.size.y * scale
				};
			}

			c_data.pos = { pos.x, pos.y };
			c_data.dim = _char_dimensions.at(c);
			c_data.texture_id = ch.texture_id;
			c_data.c = c;
			_chars->push_back(c_data);

			// Move to the next char. One advance is 1/64 of a pixel
			// the bitshifting thing gets the value in pixels (2^6 = 64)
			// If you want to learn more about this function in general, check
			// this page where most of this code portion is
			// adapter from: https://learnopengl.com/In-Practice/Text-Rendering
			x += (ch.advance >> 6) * scale;
		}

		// Sort the character data
		std::sort(_chars->begin(), _chars->end(), [](char_data a, char_data b)
		{
			return a.c < b.c;
		});
	}

	std::string text::get_text() const
	{
		return txt;
	}

	void text::clear()
	{
		txt.clear();
		_chars.reset();
		_char_dimensions.clear();
	}

	bool text::empty() const
	{
		return txt.empty();
	}

	std::shared_ptr<std::vector<text::char_data>> text::chars() const
	{
		ensure(_chars.get(), "No text has been set yet");
		return _chars;
	}

	vec2<f32> text::char_dimensions(const char c) const
	{
		return _char_dimensions.at(c);
	}
}
