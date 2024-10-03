#pragma once

#include "AspectRatioLock.hpp"
#include "Color.hpp"
#include "ColorFormat.hpp"
#include "EditorComponent.hpp"
#include "SpriteBase.hpp"

#include <memory>
#include <string>

namespace birb
{
	class texture;

	class sprite : public sprite_base, editor_component
	{
	public:
		explicit sprite(const std::string& file_path, color_format format = color_format::RGBA);
		~sprite() = default;
		sprite(const sprite& other) = default;
		sprite(sprite& other) = default;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		std::shared_ptr<birb::texture> texture;
		birb::color color;

	private:
		static inline const std::string editor_header_name = "Sprite";
	};
}
