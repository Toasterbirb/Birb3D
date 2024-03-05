#include "Material.hpp"
#include "Profiling.hpp"

namespace birb
{
	material::material(const std::string& diffuse_map, const std::string& specular_map, float shininess)
	:shininess(shininess)
	{
		assert(!diffuse_map.empty());
		assert(!specular_map.empty());

		diffuse.load(diffuse_map.c_str(), 0, color_format::RGB);
		specular.load(specular_map.c_str(), 1, color_format::RGB);
	}

	void material::apply_to_shader(shader& shader)
	{
		PROFILER_SCOPE_RENDER_FN()

		shader.activate();

		// Set the texture units
		shader.set_int("material.diffuse", 0);
		shader.set_int("material.specular", 1);

		// Apply the shininess
		shader.set_float("material.shininess", shininess);

		// Bind the textures
		diffuse.bind();
		specular.bind();
	}
}
