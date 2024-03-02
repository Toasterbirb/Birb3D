#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <string>

namespace birb
{
	class model
	{
	public:
		explicit model(const std::string& path);

		void draw(shader& shader);
	private:
		void load_model(const std::string& path);
		void process_node(aiNode* node, const aiScene* scene);
		mesh process_mesh(aiMesh* ai_mesh, const aiScene* scene);
		std::vector<mesh_texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);

		std::vector<mesh_texture> textures_loaded;
		std::vector<mesh> meshes;
		std::string directory;
	};
}
