#pragma once

#include "Color.hpp"
#include "Font.hpp"
#include "Rect.hpp"
#include "ShaderRef.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <map>

namespace birb
{
	class text
	{
	public:
		text(const std::string& text, const birb::font& font, const vec3<f32> position);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color);
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const f32 scale, const birb::color color, const shader_ref& shader);
		~text() = default;
		text(const text&) = default;
		text(text&) = default;

		struct char_data
		{
			vec2<f32> pos;
			vec2<f32> dim;
			u32 texture_id;
			char c;
		};


		birb::font font;
		vec3<f32> position;
		f32 scale;
		birb::color color;
		shader_ref shader;

		void set_text(const std::string& text);
		std::string get_text() const;
		void clear();
		bool empty() const;

		std::shared_ptr<std::vector<char_data>> chars() const;
		vec2<f32> char_dimensions(const char c) const;

	private:
		std::string txt;
		std::shared_ptr<std::vector<char_data>> _chars;
		std::map<char, vec2<f32>> _char_dimensions;
	};
}
