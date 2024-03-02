#pragma once

#include "Shader.hpp"

#include <vector>

namespace birb
{
	struct vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	struct mesh_texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class mesh
	{
	public:
		mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<mesh_texture>& textures);

		void draw(shader& shader);

		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<mesh_texture> textures;

	private:
		void setup_mesh();

		unsigned int vao, vbo, ebo;
	};
}
