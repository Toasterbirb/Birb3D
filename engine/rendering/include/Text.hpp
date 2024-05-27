#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "ShaderRef.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <unordered_map>
#include <set>

namespace birb
{
	class text
	{
	public:
		text(const std::string& text, const birb::font& font, const vec3<f32> position);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader);
		~text();
		text(const text& other);
		text(text&) = delete;

		birb::font font;
		vec3<f32> position;
		f32 scale;
		birb::color color;
		shader_ref shader;

		void set_text(const std::string& text);
		std::string get_text() const;
		void clear();
		bool empty() const;

		std::set<char> chars() const;
		std::vector<glm::vec2> char_positions(const char c) const;
		vec2<f32> char_dimensions(const char c) const;
		u32 char_texture_id(const char c) const;
		u32 instance_vbo(const char c) const;

	private:
		std::string txt;
		std::set<char> _chars;
		std::unordered_map<char, std::vector<glm::vec2>> _char_positions;
		std::unordered_map<char, vec2<f32>> _char_dimensions;
		std::unordered_map<char, u32> _char_texture_ids;

		std::unordered_map<char, u32> instance_vbos;

		void allocate_instance_vbos();
		void free_instance_vbos();
	};
}
