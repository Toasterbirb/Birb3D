#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace birb
{
	class shader;

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
		mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const std::vector<mesh_texture>& textures, const std::string& name);
		void destroy();

		void draw(shader& shader);

		std::vector<vertex> vertices;
		std::vector<u32> indices;
		std::vector<mesh_texture> textures;

		const std::string name;

	private:
		void setup_mesh();

		u32 vao, vbo, ebo;
	};
}
