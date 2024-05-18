#pragma once

#include "Color.hpp"
#include "DebugView.hpp"
#include "EventBus.hpp"
#include "FBO.hpp"
#include "ShaderRef.hpp"
#include "Types.hpp"
#include "VAO.hpp"

#include <entt.hpp>
#include <glm/fwd.hpp>

namespace birb
{
	class camera;
	class ebo;
	class scene;
	class vbo;
	class window;

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

		/**
		 * @brief Set the window for post-processing purposes
		 *
		 * The window needs to be set before post-processing can be enabled
		 */
		void set_window(birb::window& window);

		void draw_entities(const camera& camera, vec2<i32> window_size);
		void draw_elements(vao& vao, size_t index_count, gl_primitive primitive = renderer::gl_primitive::triangles);
		void draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		static void toggle_wireframe();
		static bool is_wireframe_enabled();

		static void set_backface_culling(bool state);
		static bool is_backface_culling_enabled();

		debug_view debug;
		void toggle_debug_view();

		/**
		 * @return The amount of entities rendered during last call to draw_entities()
		 */
		u32 rendered_entities_count() const;

		/**
		 * @return The amount of vertices rendered during last call to draw_entities()
		 */
		u32 rendered_vertex_count() const;

		/**
		 * @return The amount of 2D entities rendered during last call to draw_entities()
		 */
		u32 rendered_2d_entities_count() const;

		/**
		 * @return The amount of vertices rendered for 2D entities during last call to draw_entities()
		 */
		u32 rendered_2d_vertices_count() const;

		/**
		 * @return The amount of 3D entities rendered during last call to draw_entities()
		 */
		u32 rendered_3d_entities_count() const;

		/**
		 * @return The amount of vertices rendered for 3D entities during last call to draw_entities()
		 */
		u32 rendered_3d_vertices_count() const;

		/**
		 * @return The amount of screenspace entities rendered during last call to draw_entities()
		 */
		u32 rendered_screenspace_entities_count() const;

		/**
		 * @return The amount of vertices rendered for screenspace entities during last call to draw_entities()
		 */
		u32 rendered_screenspace_vertices_count() const;

		///////////////////////
		// Renderer settings //
		///////////////////////

		/**
		 * @brief OpenGL alpha blending
		 */
		void opt_blend(const bool enabled) const;

		/**
		 * @brief Post-processing render pass
		 */
		void opt_post_process(const bool enabled);

	private:
		/////////////////////////////////////////////////////////////////////////
		// The different draw functions are defined in their own source files:
		//   2d				renderer_2d.cpp
		//   3d				renderer_3d.cpp
		//   screenspace	renderer_screenspace.cpp

		void draw_2d_entities(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection);
		void draw_3d_entities(const glm::mat4& view_matrix, const glm::mat4& perspective_projection);
		void draw_screenspace_entities(const glm::mat4& orthographic_projection);

		// 2D drawing funcs
		void draw_sprites(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection);

		// 3D drawing funcs
		void draw_models(const glm::mat4& view_matrix, const glm::mat4& perspective_projection);
		void draw_box_collider_view(const glm::mat4& view_matrix, const glm::mat4& perspective_projection);

		// screenspace drawing funcs
		void draw_text(const glm::mat4& orthographic_projection_matrix);

		/////////////////////////////////////////////////////////////////////////


		scene* current_scene = nullptr;
		static inline bool wireframe_mode = false;
		static inline bool backface_culling_enabled = false;
		bool debug_view_enabled = false;

		// Variables for rendering statistics
		struct render_stat
		{
			u32 entities = 0;
			u32 vertices = 0;

			void reset()
			{
				entities = 0;
				vertices = 0;
			}
		};

		render_stat stat_total;
		render_stat stat_2d;
		render_stat stat_3d;
		render_stat stat_screenspace;


		// Counter used for avoiding unnecessary shader calculations
		u32 frame_id_counter = 0;

		/////////////////////////////////
		// Variables for debug drawing //
		/////////////////////////////////

		shader_ref debug_shader_ref;

		// Cube coords found from here
		// https://learnopengl.com/code_viewer.php?code=getting-started/cube_vertices
		static constexpr f32 collider_cube_size_offset = 0.001f;
		static constexpr std::array<f32, 36 * 3> cube_vertices = {
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
		std::unique_ptr<vbo> collider_debug_vbo;


		////////////////////////////////////
		// Variables for sprite rendering //
		////////////////////////////////////

		shader_ref texture_shader_ref;

		// Vertices and texture coordinates for a square
		static constexpr std::array<f32, 4 * 3 + 4 * 2> quad_vertices = {
			// Vertex positions  // Texture coordinates
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // Top right
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // Top left
		};

		// Indices for creating a square from two triangles
		static constexpr std::array<u32, 6> quad_indices = {
			2, 1, 0,
			3, 2, 0,
		};

		// The opengl buffers needed for sprites
		//
		// Some of these are declared as pointers because they
		// don't have default constructors
		vao sprite_vao;
		std::unique_ptr<ebo> sprite_ebo;
		std::unique_ptr<vbo> sprite_vbo;


		////////////////////
		// Text rendering //
		////////////////////

		vao text_vao;

		// The text vbo will be handled manually since it
		// involves dynamically changing the vertex data
		// and the current VBO class doesn't support that
		u32 text_vbo;


		/////////////////////
		// Post-processing //
		/////////////////////

		shader_ref post_processing_shader_ref;

		static constexpr std::array<f32, 4 * 3 + 4 * 2> fullscreen_quad_vertices = {
			// Vertex positions  // Texture coordinates
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // Top right
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom right
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom left
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Top left
		};

		vao post_processing_vao;
		std::unique_ptr<vbo> post_processing_vbo;
		// No need for ebo because we can reuse the sprite ebo

		bool post_processing_enabled = false;
		std::unique_ptr<birb::fbo> post_processing_fbo;
		birb::window* window = nullptr;
		vec2<i32> old_window_dimensions;
	};
}
