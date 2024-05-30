#pragma once

#include "Color.hpp"
#include "DebugView.hpp"
#include "EventBus.hpp"
#include "FBO.hpp"
#include "MimicSprite.hpp"
#include "RendererStats.hpp"
#include "ShaderRef.hpp"
#include "Sprite.hpp"
#include "Types.hpp"
#include "UBO.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <entt.hpp>
#include <glm/fwd.hpp>
#include <type_traits>

namespace birb
{
	class camera;
	class ebo;
	class scene;
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

		enum class gl_type
		{
			unsigned_int = 5125,
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

		/**
		 * @brief Call glDrawElements() while using the previously bound VAO
		 */
		void draw_elements(size_t index_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		/**
		 * @brief Change VAO and call glDrawElements()
		 */
		void draw_elements(vao& vao, size_t index_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		/**
		 * @brief Call glDrawElementsInstanced() while using the previously bound VAO
		 */
		void draw_elements_instanced(const u32 index_count, const u32 instance_count);

		/**
		 * @brief Call glDrawElementsInstanced() while using the previously bound VAO
		 */
		void draw_elements_instanced(const std::vector<u32>& indices, const u32 instance_count);

		/**
		 * @brief Change VAO and call glDrawElementsInstanced()
		 */
		void draw_elements_instanced(vao& vao, const std::vector<u32>& indices, const u32 instance_count);

		/**
		 * @brief Call glDrawArrays() while using the previously bound VAO
		 */
		void draw_arrays(size_t vert_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		/**
		 * @brief Change VAO and call glDrawArrays()
		 */
		void draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive = renderer::gl_primitive::triangles);

		static void toggle_wireframe();
		static bool is_wireframe_enabled();

		static void set_backface_culling(bool state);
		static bool is_backface_culling_enabled();

		debug_view debug;
		void toggle_debug_view();

		renderer_stats rendering_statistics() const;

		///////////////////////
		// Renderer settings //
		///////////////////////

		/**
		 * @brief Enable shadows for the 3D rendering pass
		 */
		void opt_shadows(const bool enabled);

		/**
		 * @brief OpenGL alpha blending
		 */
		void opt_blend(const bool enabled) const;

		/**
		 * @brief OpenGL gamma correction
		 */
		void opt_gamma_correction(const bool enabled) const;

		/**
		 * @brief Post-processing render pass
		 */
		void opt_post_process(const bool enabled);
		bool is_post_processing_enabled() const;

	private:
		/////////////////////////////////////////////////////////////////////////
		// The different draw functions are defined in their own source files:
		//   2d				renderer_2d.cpp
		//   3d				renderer_3d.cpp
		//   screenspace	renderer_screenspace.cpp

		void draw_3d_entities();
		void draw_2d_entities();
		void draw_screenspace_entities();

		// 2D drawing funcs
		template<typename T>
		void set_sprite_aspect_ratio_uniforms(const T& sprite, shader& texture_shader)
		{
			static_assert(std::is_same<T, birb::sprite>::value || std::is_same<T, birb::mimic_sprite>::value);
			if (sprite.ignore_aspect_ratio)
				texture_shader.set(shader_uniforms::texture::aspect_ratio, { 1.0f, 1.0f });
			else
				// Modify the sprite shape based on if we want to respect the aspect ratio width or height wise
				if (sprite.aspect_ratio_lock == aspect_ratio_lock::width)
					texture_shader.set(shader_uniforms::texture::aspect_ratio, { sprite.texture->aspect_ratio(), 1.0f });
				else
					texture_shader.set(shader_uniforms::texture::aspect_ratio, { 1.0f, sprite.texture->aspect_ratio_reverse() });
		}
		void draw_sprites(std::shared_ptr<shader> texture_shader);
		void draw_sprites_instanced(std::shared_ptr<shader> texture_shader);
		void draw_mimic_sprites(std::shared_ptr<shader> texture_shader);

		// 3D drawing funcs
		void draw_models();
		void draw_box_collider_view();

		// screenspace drawing funcs
		void draw_lines();
		void draw_text();

		/////////////////////////////////////////////////////////////////////////


		scene* current_scene = nullptr;
		static inline bool wireframe_mode = false;
		static inline bool backface_culling_enabled = false;
		bool debug_view_enabled = false;
		bool shadows_enabled = true;
		shader_ref line_shader;

		// Uniform buffer objects //
		birb::ubo view_matrix_ubo;

		renderer_stats render_stats;

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
		vbo text_vbo;


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
