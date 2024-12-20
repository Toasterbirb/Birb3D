#pragma once

#include "EditorComponent.hpp"
#include "ShaderRef.hpp"
#include "SpriteBase.hpp"

#include <string>

namespace birb
{
	class shader_sprite : public sprite_base, editor_component
	{
	public:
		explicit shader_sprite(const std::string& fragment_shader_name);
		explicit shader_sprite(const shader_ref shader);
		~shader_sprite() = default;
		shader_sprite(const shader_sprite&) = default;
		shader_sprite(shader_sprite&) = default;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		shader_ref shader_reference();

	private:
		static inline const std::string editor_header_name = "Shader sprite";

		shader_ref _shader_reference;
	};
}
