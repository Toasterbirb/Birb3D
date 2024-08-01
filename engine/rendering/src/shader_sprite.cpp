#include "ShaderCollection.hpp"
#include "ShaderSprite.hpp"

namespace birb
{
	constexpr char texture_vertex_shader_name[] = "texture";

	shader_sprite::shader_sprite(const std::string& fragment_shader_name)
	:shader_reference(texture_vertex_shader_name, fragment_shader_name)
	{
		// Register the shader if it hasn't been registered already
		if (!shader_collection::is_shader_registered(shader_reference))
			shader_collection::register_shader(texture_vertex_shader_name, fragment_shader_name);
	}
}
