#include "Assert.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "RendererStats.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <imgui.h>
#include <imgui_stdlib.h>

#ifndef NDEBUG
#include <filesystem> // Needed for asserts
#endif

namespace birb
{
	model::model()
	{
		textures_loaded = std::make_shared<std::vector<mesh_texture>>();
		meshes = std::make_shared<std::vector<mesh>>();
	}

	model::model(const std::string& path)
	{
		textures_loaded = std::make_shared<std::vector<mesh_texture>>();
		meshes = std::make_shared<std::vector<mesh>>();
		load_model(path);
	}

	model::~model()
	{
	}

	void model::draw(shader& shader, renderer_stats& render_stats, const bool skip_materials)
	{
		ensure(!meshes->empty(), "Attempted to draw a model with no meshes");

		for (size_t i = 0; i < meshes->size(); ++i)
		{
			meshes->at(i).draw(shader, render_stats, skip_materials);
		}
	}

	void model::draw_editor_ui()
	{
		PROFILER_SCOPE_RENDER_FN();

		static const ImVec4 red(0.80f, 0.27f, 0.27f, 1.0f);

		ImGui::BeginTable("Model info", 2);
		{
			draw_info_table_row("Meshes", meshes->size());
			draw_info_table_row("Textures loaded", textures_loaded->size());
			draw_info_table_row("File path", file_path);
			draw_info_table_row("Directory", directory);
		}
		ImGui::EndTable();

		if (ImGui::TreeNode("Meshes"))
		{
			for (size_t i = 0; i < meshes->size(); ++i)
			{
				const std::string node_name = "Mesh " + std::to_string(i);
				if (ImGui::TreeNode(node_name.c_str()))
				{
					const std::string table_name = "Mesh " + std::to_string(i) + " info";
					ImGui::BeginTable(table_name.c_str(), 2);
					draw_info_table_row("Vertices", meshes->at(i).vertices.size());
					draw_info_table_row("Indices", meshes->at(i).indices.size());
					draw_info_table_row("Textures", meshes->at(i).textures.size());
					ImGui::EndTable();
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::InputText("File path", &text_box_model_file_path);

		if (ImGui::Button("Reload"))
		{
			if (!std::filesystem::exists(text_box_model_file_path))
			{
				file_exists = false;
			}
			else
			{
				destroy();
				load_model(text_box_model_file_path);
			}
		}

		if (!file_exists)
			ImGui::TextColored(red, "File doesn't exist");
	}

	std::string model::collapsing_header_name() const
	{
		return editor_header_name;
	}

	std::string model::model_file_path()
	{
		return file_path;
	}

	void model::load_model()
	{
		ensure(!file_path.empty());

		load_model(file_path);
	}

	void model::load_model(const std::string& path)
	{
		PROFILER_SCOPE_IO_FN();

		ensure(!path.empty());

		// If the file path is a null_path, load the model from memory
		// (assuming the model was serialized from entity data)
		if (is_primitive_mesh)
		{
			load_model_from_memory(mesh_data_index, mesh_data_name);
			return;
		}

		ensure(path != null_path, "Tried to load a model from disk that was probably meant to be loaded from memory");
		ensure(std::filesystem::exists(path));

		birb::log("Loading model: " + path);

		file_exists = true;
		file_path = path;
		text_box_model_file_path = path;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			birb::log_error("assimp error: ", importer.GetErrorString());
			return;
		}

		size_t last_slash = path.find_last_of('/');

		if (last_slash != std::string::npos)
			directory = path.substr(0, last_slash);
		else
			directory = "./";

		// Reset the vert counter. The process_node function will recalculate it
		vert_count = 0;

		process_node(scene->mRootNode, scene);

		birb::log("Model loaded: " + path);
		last_write_time = std::filesystem::last_write_time(path);
	}

	void model::load_model_from_memory(const primitive_mesh mesh, const std::string& name)
	{
		PROFILER_SCOPE_IO_FN();

		ensure(primitive_mesh_data.contains(mesh), "Mesh was not found from primitive mesh data hashmap");

		birb::log("Loading a model from memory: ", name);

		this->mesh_data_index = mesh;
		this->mesh_data_name = name;
		this->is_primitive_mesh = true;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(primitive_mesh_data[mesh_data_index].data(), primitive_mesh_data[mesh_data_index].size(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			birb::log_error("assimp error: ", importer.GetErrorString());
			return;
		}

		file_path = null_path;
		directory = "./";

		// Reset the vert counter. The process_node function will recalculate it
		vert_count = 0;

		process_node(scene->mRootNode, scene);

		birb::log("Model loaded from memory: " + name);
	}

	void model::reload()
	{
		// Don't reload the model if the file has not
		// been modified
		std::filesystem::file_time_type new_last_write_time = std::filesystem::last_write_time(file_path);

		// Reload the model
		if (new_last_write_time == last_write_time)
			return;

		destroy();
		load_model();
	}

	void model::destroy()
	{
		textures_loaded->clear();

		// Destroy meshes manually
		for (birb::mesh& mesh : *meshes)
			mesh.destroy();

		meshes->clear();
		directory = "";
		vert_count = 0;
		birb::log("Model destroyed: " + file_path);
	}

	void model::process_node(aiNode* node, const aiScene* scene)
	{
		ensure(node != nullptr);
		ensure(scene != nullptr);

		// Process all meshes in the node
		for (u32 i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes->push_back(process_mesh(mesh, scene));
			vert_count += mesh->mNumVertices;
		}

		// Process the child nodes of the node
		for (u32 i = 0; i < node->mNumChildren; ++i)
		{
			process_node(node->mChildren[i], scene);
		}
	}

	mesh model::process_mesh(aiMesh* ai_mesh, const aiScene* scene)
	{
		PROFILER_SCOPE_IO_FN();

		ensure(ai_mesh != nullptr);
		ensure(scene != nullptr);

		std::vector<vertex> vertices;
		std::vector<u32> indices;
		std::vector<mesh_texture> textures;

		// Process vertices
		for (u32 i = 0; i < ai_mesh->mNumVertices; ++i)
		{
			vertex vertex;

			// Vertex position
			vertex.position.x = ai_mesh->mVertices[i].x;
			vertex.position.y = ai_mesh->mVertices[i].y;
			vertex.position.z = ai_mesh->mVertices[i].z;

			// Vertex normals
			vertex.normal.x = ai_mesh->mNormals[i].x;
			vertex.normal.y = ai_mesh->mNormals[i].y;
			vertex.normal.z = ai_mesh->mNormals[i].z;

			// Texture coordinates
			if (ai_mesh->mTextureCoords[0] != nullptr)
			{
				vertex.tex_coords.x = ai_mesh->mTextureCoords[0][i].x;
				vertex.tex_coords.y = ai_mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.tex_coords.x = 0.0f;
				vertex.tex_coords.y = 0.0f;
			}

			vertices.push_back(vertex);
		}

		// Process indices
		for (u32 i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			aiFace face = ai_mesh->mFaces[i];
			for (u32 j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// process materials
		material birb_material;
		std::string material_name;
		if (ai_mesh->mMaterialIndex > 0)
		{
			aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

			// Load material colors
			aiColor4D ai_diffuse;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &ai_diffuse);

			aiColor4D ai_specular;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &ai_specular);

			float ai_shininess;
			aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &ai_shininess);

			birb_material.diffuse = color(ai_diffuse.r, ai_diffuse.g, ai_diffuse.b, ai_diffuse.a);
			birb_material.specular = color(ai_specular.r, ai_specular.g, ai_specular.b, ai_specular.a);
			birb_material.shininess = ai_shininess;

			material_name = material->GetName().C_Str();

			// Diffuse maps
			std::vector<mesh_texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

			// Specular maps
			std::vector<mesh_texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		}

		return birb::mesh(vertices, indices, textures, birb_material, material_name, ai_mesh->mName.C_Str());
	}

	std::vector<mesh_texture> model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name)
	{
		PROFILER_SCOPE_IO_FN();

		ensure(mat != nullptr);
		ensure(!type_name.empty());

		std::vector<mesh_texture> textures;

		for (u32 i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (size_t j = 0; j < textures_loaded->size(); ++j)
			{
				if (!std::strcmp(textures_loaded->at(j).path.data(), str.C_Str()))
				{
					textures.push_back(textures_loaded->at(j));
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				mesh_texture texture;
				texture.id = texture::texture_from_file(directory + '/' + str.C_Str());
				texture.type = type_name;
				texture.path = str.C_Str();

				textures.push_back(texture);
				textures_loaded->push_back(texture);
			}
		}

		return textures;
	}

	u32 model::vertex_count() const
	{
		return vert_count;
	}
}
