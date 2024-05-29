#include "Assert.hpp"
#include "Character.hpp"
#include "ColorFormat.hpp"
#include "Globals.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "Text.hpp"

#include <GL/gl.h>
#include <string>

namespace birb
{
	const std::string default_vert_shader = "text";
	const std::string default_frag_shader = "text";

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position)
	:font(font), position(position), scale(1.0f), color(0xFFFFFF), shader(default_vert_shader, default_frag_shader), fbo({1, 1}, color_format::RGBA)
	{
		setup_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale)
	:font(font), position(position), scale(scale), color(0xFFFFFF), shader(default_vert_shader, default_frag_shader), fbo({1, 1}, color_format::RGBA)
	{
		setup_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color)
	:font(font), position(position), scale(scale), color(color), shader(default_vert_shader, default_frag_shader), fbo({1, 1}, color_format::RGBA)
	{
		setup_text(text);
	}

	text::text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader)
	:font(font), position(position), scale(scale), color(color), shader(shader), fbo({1, 1}, color_format::RGBA)
	{
		setup_text(text);
	}

	text::~text()
	{
		event_bus::unregister_event_id(event::window_resized, this);
		free_instance_vbos();
	}

	void text::process_event(u16 event_id, const event_data& data)
	{
		switch (event_id)
		{
			case (event::window_resized):
				set_text(txt);
				break;

			default:
				break;
		}
	}

	text::text(const text& other)
	:font(other.font),
	 position(other.position),
	 color(other.color),
	 shader(other.shader),
	 fbo({1, 1}, color_format::RGBA),
	 txt(other.txt)
	{
		setup_text(other.txt);
	}

	void text::set_text(const std::string& text)
	{
		PROFILER_SCOPE_RENDER_FN();

		// Clear out any previous string data
		clear();

		this->txt = text;

		if (this->txt.empty())
			return;

		// Characters for keeping track of the dimensions
		// of the characters at the edges of the text block
		char max_x_c = 0;
		char max_y_c = 0;

		f32 text_block_width = 0;
		f32 text_block_height = 0;

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

			// Keep track of the character position on the x-axis to find the one
			// that is at the right most edge of the text block
			if (x >= text_block_width)
			{
				max_x_c = c;
				text_block_width = x;
			}

			// Move to the next char. One advance is 1/64 of a pixel
			// the bitshifting thing gets the value in pixels (2^6 = 64)
			// If you want to learn more about this function in general, check
			// this page where most of this code portion is
			// adapter from: https://learnopengl.com/In-Practice/Text-Rendering
			x += (ch.advance >> 6);
		}

		// Find the character with the largest height value
		float char_max_height = 0;
		for (const auto& dim : _char_dimensions)
			if (dim.second.y > char_max_height)
				char_max_height = dim.second.y;

		// Calculate the final dimensions
		ensure(max_x_c != 0 && max_x_c != '\n');
		text_block_width += _char_dimensions.at(max_x_c).x;
		text_block_height += std::abs(y + char_max_height); // The y value is negative

		ensure(text_block_width > 1);
		ensure(text_block_height > 1);

		// Resize the fbo frame buffer to the text block dimensions
		fbo.bind();
		fbo.reload_frame_buffer_texture(vec2<i32>( window_dimensions.x, window_dimensions.y ), color_format::RGBA);
		fbo.unbind();

		// ensure(fbo.frame_buffer_dimensions().x == std::floor(text_block_width));
		// ensure(fbo.frame_buffer_dimensions().y == std::floor(text_block_height));

		ensure(!_chars.empty(), "This code path shouldn't be reached with an empty string");
		ensure(_char_positions.size() <= txt.size());

		// Create instance vbos
		allocate_instance_vbos();

		// Create the texture atlas
		render_text_texture_atlas();

		ensure(_chars.size() == _char_dimensions.size());
		ensure(_chars.size() == _char_texture_ids.size());
		ensure(_chars.size() == instance_vbos.size());
	}

	std::string text::get_text() const
	{
		return txt;
	}

	void text::clear()
	{
		free_instance_vbos();

		// txt.clear();
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

	u32 text::atlas_id() const
	{
		return fbo.frame_buffer_id();
	}

	vec2<i32> text::atlas_dimensions() const
	{
		return fbo.frame_buffer_dimensions();
	}

	void text::setup_text(const std::string text)
	{
		this->txt = text;
		set_text(text);
		event_bus::register_event_id(event::window_resized, this);
	}

	void text::allocate_instance_vbos()
	{
		ensure(instance_vbos.empty());

		std::vector<u32> instance_vbo_arr(_chars.size());
		glGenBuffers(_chars.size(), instance_vbo_arr.data());

		u32 index = 0;
		for (const char c : _chars)
		{
			const u32 vbo = instance_vbo_arr.at(index++);

			instance_vbos[c] = vbo;

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
					sizeof(glm::vec2) * _char_positions.at(c).size(),
					_char_positions.at(c).data(),
					GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void text::free_instance_vbos()
	{
		for (const std::pair<char, u32> vbo : instance_vbos)
			glDeleteBuffers(1, &vbo.second);

		instance_vbos.clear();
	}

	void text::render_text_texture_atlas()
	{
		birb::vao vao;
		birb::vbo vbo;

		vao.bind();

		vbo.bind();
		vbo.set_data(NULL, sizeof(f32) * 6 * 4, false); // 6 verts, 4 floats per vert
		vbo.enable_vertex_attrib_array(0);
		vbo.set_vertex_attrib_ptr(0, 0, sizeof(f32), 4);

		vao.unbind();
		vbo.unbind();

		// Fetch the shader
		const std::shared_ptr<birb::shader> shader = shader_collection::get_shader(this->shader);
		ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

		shader->set(shader_uniforms::text::color, color);
		shader->set(shader_uniforms::text::position, position);

		vao.bind();
		vbo.bind();
		vbo.set_vertex_attrib_ptr(0, 0, sizeof(f32), 4);
		glActiveTexture(GL_TEXTURE0);

		fbo.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		fbo.clear();

		// Iterate through the text
		for (const char c : _chars)
		{
			// Update the VBO
			constexpr u8 vert_count = 6;

			const vec2<f32>& dim = char_dimensions(c);

			const f32 verts[vert_count][4] = {
				{ 0,		dim.y,	0.0f, 0.0f },
				{ 0,		0,		0.0f, 1.0f },
				{ dim.x, 	0,		1.0f, 1.0f },

				{ 0,		dim.y,	0.0f, 0.0f },
				{ dim.x,	0, 		1.0f, 1.0f },
				{ dim.x,	dim.y,	1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, char_texture_id(c));

			vbo.bind();
			vbo.update_data(*verts, sizeof(verts));

			glBindBuffer(GL_ARRAY_BUFFER, instance_vbo(c));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, char_positions(c).size());
		}

		vao.unbind();
		fbo.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
