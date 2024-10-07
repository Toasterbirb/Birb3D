#include "Assert.hpp"
#include "Character.hpp"
#include "GLSupervisor.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Text.hpp"

#include <glad/gl.h>
#include <imgui.h>
#include <string>

namespace birb
{
	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const birb::color color, const shader_ref& shader)
	:font(font), position(position), color(color), shader(shader)
	{
		allocate_instance_vbos();
		set_text(text);
	}

	text::~text()
	{
		for (const std::pair<char, u32> vbo : instance_vbos)
			glDeleteBuffers(1, &vbo.second);
	}

	text::text(const text& other)
	:font(other.font),
	 position(other.position),
	 color(other.color),
	 shader(other.shader),
	 txt(other.txt),
	 _chars(other._chars),
	 _char_positions(other._char_positions),
	 _char_texture_ids(other._char_texture_ids)
	{
		allocate_instance_vbos();
		update_instance_vbos();
	}

	void text::draw_editor_ui()
	{
		ImGui::Text("Text: '%s'", txt.c_str());
		ImGui::DragFloat3("Position", *position.to_ptr_array().data());
		ImGui::ColorEdit3("Color", *color.to_ptr_array().data());
	}

	std::string text::collapsing_header_name() const
	{
		return editor_header_name;
	}

	void text::set_text(const std::string& text)
	{
		PROFILER_SCOPE_RENDER_FN();

		// Clear out any previous string data
		clear();

		this->txt = text;

		f32 x = 0.0f;
		f32 y = 0.0f;

		for (const char c : txt)
		{
			// Process newline characters
			if (c == '\n')
			{
				// Go back to the starting position width wise
				x = 0;

				// Go one line down
				y -= font.size;

				// We shouldn't draw the newline char
				continue;
			}

			const character& ch = font.get_char(c);

			// position
			const glm::vec2 pos(
				x + ch.bearing.x,
				y - (ch.size.y - ch.bearing.y)
			);

			_chars.insert(c);
			_char_positions[c].push_back(pos);
			_char_texture_ids[c] = ch.texture_id;


			// Move to the next char. One advance is 1/64 of a pixel
			// the bitshifting thing gets the value in pixels (2^6 = 64)
			// If you want to learn more about this function in general, check
			// this page where most of this code portion is
			// adapted from: https://learnopengl.com/In-Practice/Text-Rendering
			x += (ch.advance >> 6);
		}

		ensure(!_chars.empty(), "This code path shouldn't be reached with an empty string");
		ensure(_char_positions.size() <= txt.size());

		// Update instance vbos
		update_instance_vbos();

		ensure(_chars.size() == _char_texture_ids.size());
	}

	std::string text::get_text() const
	{
		return txt;
	}

	void text::clear()
	{
		txt.clear();
		_chars.clear();
		_char_positions.clear();
		_char_texture_ids.clear();
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

	void text::allocate_instance_vbos()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(instance_vbos.empty());
		ensure(instance_vbo_ids.empty());

		constexpr u8 max_char_count = 128;
		instance_vbo_ids.resize(max_char_count);
		glGenBuffers(max_char_count, instance_vbo_ids.data());
	}

	void text::update_instance_vbos()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(!instance_vbo_ids.empty());

		u32 index = 0;
		for (const char c : _chars)
		{
			const u32 vbo = instance_vbo_ids.at(index++);

			instance_vbos[c] = vbo;

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
					sizeof(glm::vec2) * _char_positions.at(c).size(),
					_char_positions.at(c).data(),
					GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
