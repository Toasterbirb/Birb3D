#include "Assert.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "Profiling.hpp"
#include "RendererStats.hpp"
#include "Shader.hpp"

#include <climits>
#include <cstddef>
#include <glad/gl.h>
#include <string>

namespace birb
{
	mesh::mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const std::vector<mesh_texture>& textures, const birb::material& material, const std::string& material_name, const std::string& name)
	:vertices(vertices), indices(indices), textures(textures), material_name(material_name), material(material), name(name)
	{
		setup_mesh();
		birb::log("Mesh constructed: ", name, " (mat: ", material_name, ", addr: ", birb::ptr_to_str(this), ")");
	}

	void mesh::destroy()
	{
		birb::log("Destroying mesh (" + birb::ptr_to_str(this) + ")");

		ensure(birb::g_opengl_initialized);

		ensure(vao != 0);
		ensure(vbo != 0);
		ensure(ebo != 0);

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	void mesh::draw(shader& shader, renderer_stats& render_stats, const bool skip_materials)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(shader.id != 0);

		u32 diffuse_nr = 1;
		u32 specular_nr = 1;

		// Apply the material on the mesh if it has any
		if (!skip_materials && !material_name.empty())
			shader.apply_color_material(material);

		for (size_t i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuse_nr++);
			else if (name == "texture_specular")
				number = std::to_string(specular_nr++);
			else
				birb::log_warn("Tried to draw an unknown texture type: " + textures[i].type);

			ensure(i < INT_MAX, "Integer overflow");
			shader.set_int("material." + name + number, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// Draw the mesh
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		++render_stats.draw_elements_vao_calls;
	}

	void mesh::setup_mesh()
	{
		PROFILER_SCOPE_MISC_FN();

		ensure(!vertices.empty());
		ensure(!indices.empty());
		ensure(indices.size() >= vertices.size());

		// Create the buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		// Bind the VAO and setup the VBO with the vertex data
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

		// Bind the EBO and setup the indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), &indices[0], GL_STATIC_DRAW);

		// -- Load data into the currently bound VBO, I think ... --

		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);

		// Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, normal)));

		// Vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, tex_coords)));

		// Unbind the VAO
		glBindVertexArray(0);
	}
}
