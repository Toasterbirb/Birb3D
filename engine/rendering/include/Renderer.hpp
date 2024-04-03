#pragma once

#include "EBO.hpp"
#include "EventBus.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <entt.hpp>
#include <glm/fwd.hpp>

namespace birb
{
	class renderer : public event_obj
	{
	public:
		renderer();
		renderer(const renderer&) = delete;
		~renderer();

		enum class gl_primitive
		{
			lines = 1,
			triangles = 4,
		};

		void process_event(u16 event_id, const event_data& data) override;

		void set_scene(scene& scene);

		void draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);
		void draw_elements(vao& vao, size_t index_count, gl_primitive primitive = renderer::gl_primitive::triangles);
		void draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		static void toggle_wireframe();
		static bool is_wireframe_enabled();

		static void set_backface_culling(bool state);
		static bool is_backface_culling_enabled();

		void toggle_debug_view();

		/**
		 * @return The amount of entities rendered during last call to draw_entities()
		 */
		u32 rendered_entities_count() const;

		/**
		 * @return The amount of vertices rendered during last call to draw_entities()
		 */
		u32 rendered_vertex_count() const;

		///////////////////////
		// Renderer settings //
		///////////////////////

		/**
		 * @brief OpenGL alpha blending
		 */
		void opt_blend(const bool enabled) const;

	private:
		scene* current_scene = nullptr;
		static inline bool wireframe_mode = false;
		static inline bool backface_culling_enabled = false;
		bool debug_view_enabled = false;

		// Variables for rendering statistics
		u32 rendered_entities = 0;
		u32 rendered_vertices = 0;

		/////////////////////////////////
		// Variables for debug drawing //
		/////////////////////////////////

		// Cube coords found from here
		// https://learnopengl.com/code_viewer.php?code=getting-started/cube_vertices
		static constexpr float collider_cube_size_offset = 0.001f;
		static constexpr std::array<float, 36 * 3> cube_vertices = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		color collider_debug_color;
		vao collider_debug_vao;
		std::shared_ptr<vbo> collider_debug_vbo;


		////////////////////////////////////
		// Variables for sprite rendering //
		////////////////////////////////////

		// Vertices and texture coordinates for a square
		static constexpr std::array<float, 4 * 3 + 4 * 2> quad_vertices = {
			// Vertex positions  // Texture coordinates
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // Top right
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // Top left
		};

		// Indices for creating a square from two triangles
		static constexpr std::array<u32, 6> quad_indices = {
			0, 1, 2,
			0, 2, 3,
		};

		// The opengl buffers needed for sprites
		//
		// Some of these are declared as pointers because they
		// don't have default constructors
		vao sprite_vao;
		std::shared_ptr<ebo> sprite_ebo;
		std::shared_ptr<vbo> sprite_vbo;
	};
}
