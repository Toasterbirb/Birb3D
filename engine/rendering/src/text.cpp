#include "Assert.hpp"
#include "Character.hpp"
#include "Profiling.hpp"
#include "Text.hpp"

#include <GL/gl.h>
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

		// Clear out any previous string data
		clear();

		this->txt = text;

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
			const glm::vec2 pos(
				x + ch.bearing.x * scale,
				y - (ch.size.y - ch.bearing.y) * scale
			);

			// Avoid storing duplicated dimension data
			if (!_char_dimensions.contains(c))
			{
				_char_dimensions[c] = {
					ch.size.x * scale,
					ch.size.y * scale
				};
			}

			_chars.insert(c);
			_char_positions[c].push_back(pos);
			_char_texture_ids[c] = ch.texture_id;

			// Move to the next char. One advance is 1/64 of a pixel
			// the bitshifting thing gets the value in pixels (2^6 = 64)
			// If you want to learn more about this function in general, check
			// this page where most of this code portion is
			// adapter from: https://learnopengl.com/In-Practice/Text-Rendering
			x += (ch.advance >> 6) * scale;
		}

		ensure(!_chars.empty(), "This code path shouldn't be reached with an empty string");
		ensure(_char_positions.size() <= txt.size());

		// Create vaos and instance vbos
		std::vector<u32> instance_vbo_arr(_chars.size());
		glGenBuffers(_chars.size(), instance_vbo_arr.data());

		u32 index = 0;
		for (const char c : _chars)
		{
			const vec2<f32>& dim = char_dimensions(c);
			const u32 vbo = instance_vbo_arr.at(index++);

			instance_vbos[c] = vbo;

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
					sizeof(glm::vec2) * _char_positions.at(c).size(),
					_char_positions.at(c).data(),
					GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		ensure(_chars.size() == _char_dimensions.size());
		ensure(_chars.size() == _char_texture_ids.size());
		ensure(_chars.size() == instance_vbos.size());
		ensure(_chars.size() == instance_vbo_arr.size());
	}

	std::string text::get_text() const
	{
		return txt;
	}

	void text::clear()
	{
		// Free the vertex buffer objects
		for (const std::pair<char, u32> vbo : instance_vbos)
			glDeleteBuffers(1, &vbo.second);

		txt.clear();
		_chars.clear();
		_char_positions.clear();
		_char_dimensions.clear();
		_char_texture_ids.clear();

		instance_vbos.clear();
	}

	bool text::empty() const
	{
		return txt.empty();
	}

	std::set<char> text::chars() const
	{
		return _chars;
	}

	std::vector<glm::vec2> text::char_positions(const char c) const
	{
		ensure(_char_positions.contains(c));
		return _char_positions.at(c);
	}

	vec2<f32> text::char_dimensions(const char c) const
	{
		ensure(_char_dimensions.contains(c));
		return _char_dimensions.at(c);
	}

	u32 text::char_texture_id(const char c) const
	{
		ensure(_char_texture_ids.contains(c));
		return _char_texture_ids.at(c);
	}

	u32 text::instance_vbo(const char c) const
	{
		ensure(instance_vbos.contains(c));
		return instance_vbos.at(c);
	}
}
