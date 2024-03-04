#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "EditorComponent.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <string>

namespace birb
{
	class model : public editor_component
	{
	public:
		explicit model(const std::string& path);

		void draw(shader& shader);
		void draw_editor_ui() override;

		std::string model_file_path();
		void load_model(const std::string& path);
		void destroy();

		unsigned int vertex_count() const;

	private:
		void process_node(aiNode* node, const aiScene* scene);
		mesh process_mesh(aiMesh* ai_mesh, const aiScene* scene);
		std::vector<mesh_texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);

		std::vector<mesh_texture> textures_loaded;
		std::vector<mesh> meshes;
		std::string directory;
		std::string file_path;

		unsigned int vert_count = 0;

		// Editor stuff
		std::string text_box_model_file_path = "";
		bool file_exists = true;
	};
}
