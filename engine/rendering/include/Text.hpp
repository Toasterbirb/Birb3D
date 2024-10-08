#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"
#include "Font.hpp"
#include "ShaderRef.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <set>
#include <unordered_map>
#include <vector>

namespace birb
{
	class text : editor_component
	{
	public:
		text(const std::string& text, const birb::font& font, const vec3<f32> position, const birb::color = 0xFFFFFF, const shader_ref& = {"text", "text"});
		~text();
		text(const text& other);
		text(text&) = delete;
		text(text&&) = default;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		birb::font font;
		vec3<f32> position;
		birb::color color;
		shader_ref shader;

		void set_text(const std::string& text);
		std::string get_text() const;
		void clear();
		bool empty() const;

		std::set<char> chars() const;
		std::vector<glm::vec2> char_positions(const char c) const;
		u32 char_texture_id(const char c) const;
		u32 instance_vbo(const char c) const;

	private:
		static inline const std::string editor_header_name = "Text";
		std::string txt;
		std::set<char> _chars;
		std::unordered_map<char, std::vector<glm::vec2>> _char_positions;
		std::unordered_map<char, u32> _char_texture_ids;

		std::vector<u32> instance_vbo_ids;
		std::unordered_map<char, u32> instance_vbos;

		void allocate_instance_vbos();
		void update_instance_vbos();
	};
}
