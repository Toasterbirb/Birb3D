#pragma once

#include "GLBuffer.hpp"
#include "Material.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace birb
{
	class shader;
	struct renderer_stats;

	struct vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	struct mesh_texture
	{
		u32 id;
		std::string type;
		std::string path;
	};

	class mesh
	{
	public:
		mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const std::vector<mesh_texture>& textures, const material& material, const std::string& material_name, const std::string& name);
		void destroy();

		void draw(shader& shader, renderer_stats& render_stats, const bool skip_materials = false);

		std::vector<vertex> vertices;
		std::vector<u32> indices;
		std::vector<mesh_texture> textures;

		std::string material_name;
		birb::material material;

		/**
		 * @brief Name of the mesh
		 */
		const std::string name;

	private:
		void setup_mesh();

		gl_buffer vbo, ebo;
		u32 vao;
	};
}
