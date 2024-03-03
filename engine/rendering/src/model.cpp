#include "Mesh.hpp"
#include "Model.hpp"
#include "Logger.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#ifndef NDEBUG
#include <filesystem> // Needed for asserts
#endif

namespace birb
{
	model::model(const std::string& path)
	{
		load_model(path);
	}

	void model::draw(shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].draw(shader);
		}
	}

	void model::load_model(const std::string& path)
	{
		assert(!path.empty());
		assert(std::filesystem::exists(path));

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			birb::log_error("assimp error: " + std::string(importer.GetErrorString()));
			return;
		}

		size_t last_slash = path.find_last_of('/');

		if (last_slash != std::string::npos)
			directory = path.substr(0, last_slash);
		else
			directory = "./";

		process_node(scene->mRootNode, scene);
	}


	void model::process_node(aiNode* node, const aiScene* scene)
	{
		// Process all meshes in the node
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(process_mesh(mesh, scene));
		}

		// Process the child nodes of the node
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			process_node(node->mChildren[i], scene);
		}
	}

	mesh model::process_mesh(aiMesh* ai_mesh, const aiScene* scene)
	{
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<mesh_texture> textures;

		// Process vertices
		for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i)
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
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			aiFace face = ai_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// process materials
		if (ai_mesh->mMaterialIndex > 0)
		{
			aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

			// Diffuse maps
			std::vector<mesh_texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

			// Specular maps
			std::vector<mesh_texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		}

		return birb::mesh(vertices, indices, textures);
	}

	std::vector<mesh_texture> model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name)
	{
		std::vector<mesh_texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); ++j)
			{
				if (!std::strcmp(textures_loaded[j].path.data(), str.C_Str()))
				{
					textures.push_back(textures_loaded[j]);
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
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}
}