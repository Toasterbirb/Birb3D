#include "ShaderCollection.hpp"
#include "ShaderSprite.hpp"

namespace birb
{
	constexpr char texture_vertex_shader_name[] = "texture";

	shader_sprite::shader_sprite(const std::string& fragment_shader_name)
	:_shader_reference(texture_vertex_shader_name, fragment_shader_name)
	{
		// Register the shader if it hasn't been registered already
		if (!shader_collection::is_shader_registered(_shader_reference))
			shader_collection::register_shader(texture_vertex_shader_name, fragment_shader_name);
	}

	shader_sprite::shader_sprite(const shader_ref shader)
	:_shader_reference(shader)
	{}

	void shader_sprite::draw_editor_ui()
	{
		// draw the ui for the sprite base class
		draw_editor_base_ui();
		ImGui::Separator();
		_shader_reference.draw_editor_ui();
	}

	std::string shader_sprite::collapsing_header_name() const
	{
		return editor_header_name;
	}

	shader_ref shader_sprite::shader_reference()
	{
		return _shader_reference;
	}
}
