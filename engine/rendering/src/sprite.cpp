#include "Profiling.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"

namespace birb
{
	sprite::sprite(const std::string& file_path, color_format format)
	:color(1.0f, 1.0f, 1.0f, 1.0f)
	{
		PROFILER_SCOPE_RENDER_FN();

		texture = std::make_shared<birb::texture>(file_path.c_str(), 0, format, texture_type::TEX_2D);
	}

	void sprite::draw_editor_ui()
	{
		// draw the ui for the sprite base class
		draw_editor_base_ui();

		// TODO: draw texture UI here

		ImGui::ColorEdit4("Color", *color.to_ptr_array_alpha().data());
	}

	std::string sprite::collapsing_header_name() const
	{
		return editor_header_name;
	}
}
