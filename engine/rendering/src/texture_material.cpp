#include "TextureMaterial.hpp"
#include "Profiling.hpp"

namespace birb
{
	texture_material::texture_material(const std::string& diffuse_map, const std::string& specular_map, float shininess)
	:shininess(shininess)
	{
		assert(!diffuse_map.empty());
		assert(!specular_map.empty());

		diffuse.load(diffuse_map.c_str(), 0, color_format::RGB);
		specular.load(specular_map.c_str(), 1, color_format::RGB);
	}

	void texture_material::apply_to_shader(shader& shader)
	{
		PROFILER_SCOPE_RENDER_FN()

		shader.activate();

		// Set the texture units
		shader.set(shader_uniforms::material_texture::diffuse, 0);
		shader.set(shader_uniforms::material_texture::specular, 1);

		// Apply the shininess
		shader.set(shader_uniforms::material_texture::shininess, shininess);

		// Bind the textures
		diffuse.bind();
		specular.bind();
	}
}
