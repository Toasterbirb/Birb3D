#pragma once

#include "EditorComponent.hpp"
#include "Mesh.hpp"
#include "PrimitiveMeshes.hpp"
#include "Shader.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <string>

namespace birb
{
	class model : public editor_component
	{
	public:
		model();
		explicit model(const std::string& path);
		~model();

		void draw(shader& shader);
		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		std::string model_file_path();
		void load_model();
		void load_model(const std::string& path);
		void load_model_from_memory(const primitive_mesh mesh, const std::string& name = "unknown");
		void destroy();

		unsigned int vertex_count() const;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(directory, file_path, is_primitive_mesh, mesh_data_index, mesh_data_name);
		}

	private:
		const std::string editor_header_name = "Model";

		void process_node(aiNode* node, const aiScene* scene);
		mesh process_mesh(aiMesh* ai_mesh, const aiScene* scene);
		std::vector<mesh_texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);

		std::vector<mesh_texture> textures_loaded;
		std::vector<mesh> meshes;
		std::string directory;
		std::string file_path;

		// Mesh data for built in primitive meshes
		//
		// These will be empty for normal models and only used
		// when the file_path is equal to the null_path
		bool is_primitive_mesh = false;
		primitive_mesh mesh_data_index;
		std::string mesh_data_name = "unknown";

		// Files with this path don't exist
		static inline const std::string null_path = "...";

		unsigned int vert_count = 0;

		// Editor stuff
		std::string text_box_model_file_path = "";
		bool file_exists = true;
	};
}
