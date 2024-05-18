#include "Assert.hpp"
#include "Character.hpp"
#include "EBO.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "Sprite.hpp"
#include "State.hpp"
#include "Text.hpp"
#include "Transform.hpp"
#include "VBO.hpp"
#include "Window.hpp"

// Debug drawing dependencies
#include "BoxCollider.hpp"

#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <cstdint>
#include <entt.hpp>
#include <glad/gl.h>
#include <memory>
#include <unordered_set>

// Make sure that all of the datatypes are of correct size
// so that they work correctly with OpenGL
static_assert(sizeof(f64) == sizeof(GLdouble));
static_assert(sizeof(f32) == sizeof(GLfloat));
static_assert(sizeof(int) == sizeof(GLint));
#ifdef BIRB_PLATFORM_LINUX
static_assert(sizeof(long int) == sizeof(GLint64));
#else
#pragma message "WARNING: long int != GLint64 with mingw"
#endif
static_assert(sizeof(short) == sizeof(GLshort));
static_assert(sizeof(std::byte) == sizeof(GLbyte));
static_assert(sizeof(unsigned int) == sizeof(GLuint));
#ifdef BIRB_PLATFORM_LINUX
static_assert(sizeof(unsigned long int) == sizeof(GLuint64));
#else
#pragma message "WARNING: unsigned long int != GLint64 with mingw"
#endif
static_assert(sizeof(unsigned short) == sizeof(GLushort));
static_assert(sizeof(size_t) == sizeof(GLsizeiptr));
static_assert(sizeof(intptr_t) == sizeof(GLintptr));

static_assert(sizeof(i8) == sizeof(GLchar));
static_assert(sizeof(i16) == sizeof(GLshort));
static_assert(sizeof(i32) == sizeof(GLint));
static_assert(sizeof(i64) == sizeof(GLint64));
static_assert(sizeof(u8) == sizeof(GLchar));
static_assert(sizeof(u16) == sizeof(GLushort));
static_assert(sizeof(u32) == sizeof(GLuint));
static_assert(sizeof(u64) == sizeof(GLuint64));

static_assert(static_cast<GLenum>(birb::renderer::gl_primitive::triangles) == GL_TRIANGLES);
static_assert(static_cast<GLenum>(birb::renderer::gl_primitive::lines) == GL_LINES);

namespace birb
{
	renderer::renderer()
	:debug_shader_ref("color", "color"), texture_shader_ref("texture", "texture"), post_processing_shader_ref("post_process", "post_process")
	{
		event_bus::register_event_id(event::toggle_wireframe_rendering_mode, this);
		event_bus::register_event_id(event::toggle_debug_view, this);

		// Initialize the opengl buffers for sprite rendering
		sprite_vao.bind();

		sprite_vbo = std::make_unique<vbo>(quad_vertices);
		sprite_ebo = std::make_unique<ebo>(quad_indices);

		constexpr u32 vertex_pos_layout = 0;
		constexpr u32 texture_coord_layout = 1;

		constexpr u32 vertex_component_count = 3;
		constexpr u32 texture_coord_component_count = 2;

		constexpr u32 vertex_offset = 0;
		constexpr u32 texture_coord_offset = vertex_component_count;

		constexpr u32 stride = 5;

		sprite_vao.link_vbo(*sprite_vbo,
				vertex_pos_layout,
				vertex_component_count,
				stride,
				vertex_offset);

		sprite_vao.link_vbo(*sprite_vbo,
				texture_coord_layout,
				texture_coord_component_count,
				stride,
				texture_coord_offset);

		sprite_vao.unbind();
		sprite_vbo->unbind();
		sprite_ebo->unbind();


		// Initialize things for text rendering
		glGenBuffers(1, &text_vbo);
		text_vao.bind();
		glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // 6 verts, 4 floats per vert
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		text_vao.unbind();


		// Initialize things for the collider debug cube
		collider_debug_color = 0xC34043;

		collider_debug_vao.bind();

		collider_debug_vbo = std::make_unique<vbo>(cube_vertices);
		collider_debug_vao.link_vbo(*collider_debug_vbo, 0, 3, 3, 0);
		collider_debug_vao.unbind();
		collider_debug_vbo->unbind();


		// Initialize things for post-processing
		post_processing_fbo = std::make_unique<fbo>(vec2<i32>(1920, 720));
		post_processing_vbo = std::make_unique<vbo>(fullscreen_quad_vertices);

		post_processing_vao.bind();
		post_processing_vao.link_ebo(*sprite_ebo);
		post_processing_vao.link_vbo(*post_processing_vbo, 0, 3, 5, 0);
		post_processing_vao.link_vbo(*post_processing_vbo, 1, 2, 5, 3);
		post_processing_vao.unbind();
		sprite_ebo->unbind();

		// Precompile shaders in release builds
#ifdef NDEBUG
		shader_collection::precompile_basic_shaders();
#else
		birb::log_warn("Shader precompiling is disabled in debug builds");
#endif
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
		event_bus::unregister_event_id(event::toggle_wireframe_rendering_mode, this);
		event_bus::unregister_event_id(event::toggle_debug_view, this);
	}

	void renderer::process_event(u16 event_id, const event_data& data)
	{
		switch (event_id)
		{
			case event::toggle_wireframe_rendering_mode:
				toggle_wireframe();
				break;

			case event::toggle_debug_view:
				toggle_debug_view();
				break;
		}
	}

	void renderer::set_scene(scene& scene)
	{
		ensure(scene::scene_count() > 0);
		current_scene = &scene;
	}

	void renderer::set_window(birb::window& window)
	{
		this->window = &window;
	}

	void renderer::draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		PROFILER_SCOPE_RENDER_FN()

		ensure(current_scene != nullptr);
		ensure(scene::scene_count() > 0);
		ensure(!buffers_flipped, "Tried to draw entities after the buffers were already flipped");

		// Bind the post-processing frame buffer and update its dimensions if needed
		if (post_processing_enabled)
		{
			if (window->size() != old_window_dimensions)
			{
				post_processing_fbo->reload_frame_buffer_texture(window->size());
				old_window_dimensions = window->size();
			}

			post_processing_fbo->bind();

			// The FBO needs to be cleared separately
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		entt::registry& entity_registry = current_scene->registry;

		rendered_entities = 0;
		rendered_vertices = 0;

		// Render all models
		{
			PROFILER_SCOPE_RENDER("Render models")

			// Store shaders that have some specific uniforms updated already
			// to avoid duplicate uniform updates
			std::unordered_set<u32> uniforms_updated;

			const auto view = entity_registry.view<birb::model, birb::shader_ref, birb::transform>();
			for (const auto& ent : view)
			{
				// Check if the entity should be skipped because its not active
				const state* state = entity_registry.try_get<birb::state>(ent);
				if (state && !state->active)
					continue;

				// Get the shader we'll be using for drawing the meshes of the model
				shader_ref& shader_reference = view.get<birb::shader_ref>(ent);
				std::shared_ptr<shader> shader = shader_collection::get_shader(shader_reference);

				ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

				const birb::transform& transform = view.get<birb::transform>(ent);

				// Make sure the lighting is up-to-date
				if (!uniforms_updated.contains(shader->id))
				{
					shader->set(shader_uniforms::view, view_matrix);
					shader->set(shader_uniforms::projection, projection_matrix);

					shader->update_directional_light();
					shader->update_point_lights();
					uniforms_updated.insert(shader->id);
				}

				shader->set(shader_uniforms::model, transform.model_matrix());

				// Apply the material component on the shader if it has any
				// TODO: Make this work with textures too
				const birb::material* material = entity_registry.try_get<birb::material>(ent);
				if (material != nullptr)
					shader->apply_color_material(*material);

				// Draw the model
				ensure(view.get<birb::model>(ent).vertex_count() != 0, "Tried to render a model with no vertices");
				view.get<birb::model>(ent).draw(*shader);
				++rendered_entities;
				rendered_vertices += view.get<birb::model>(ent).vertex_count();
			}
		}

		// Render sprites
		{
			PROFILER_SCOPE_RENDER("Render sprites")

			const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(texture_shader_ref);

			// Sprites should only have a singular texture, so we'll use the default
			// tex0 texture unit
			texture_shader->set(shader_uniforms::texture_units::tex0, 0);

			texture_shader->set(shader_uniforms::view, view_matrix);
			texture_shader->set(shader_uniforms::projection, projection_matrix);
			sprite_vao.bind();


			const auto view = entity_registry.view<sprite, transform>();

			// Calculate model matrices in parallel
			std::vector<glm::mat4> model_matrices(std::distance(view.begin(), view.end()));

			{
				PROFILER_SCOPE_RENDER("Calculate transform model matrices")

				std::transform(std::execution::par, view.begin(), view.end(), model_matrices.begin(), [view](const auto& entity) {
					return view.get<birb::transform>(entity).model_matrix();
				});
			}

			size_t sprite_index = 0;
			texture_shader->activate();
			sprite_vao.bind();

			for (const auto& ent : view)
			{
				texture_shader->set(shader_uniforms::model, model_matrices[sprite_index++]);

				sprite& entity_sprite = view.get<birb::sprite>(ent);

				if (entity_sprite.ignore_aspect_ratio)
				{
					texture_shader->set(shader_uniforms::texture::aspect_ratio, 1.0f);
					texture_shader->set(shader_uniforms::texture::aspect_ratio_reverse, 1.0f);
				}
				else
				{
					// Modify the sprite shape based on if we wan't to respect the aspect ratio width or height wise
					if (entity_sprite.aspect_ratio_lock == sprite::aspect_ratio_lock::width)
					{
						texture_shader->set(shader_uniforms::texture::aspect_ratio, entity_sprite.texture->aspect_ratio());
						texture_shader->set(shader_uniforms::texture::aspect_ratio_reverse, 1.0f);
					}
					else
					{
						texture_shader->set(shader_uniforms::texture::aspect_ratio, 1.0f);
						texture_shader->set(shader_uniforms::texture::aspect_ratio_reverse, entity_sprite.texture->aspect_ratio_reverse());
					}
				}

				entity_sprite.texture->bind();

				// Since we are using the same vao for all of the sprites,
				// we can just manually call glDrawElements without binding the vao for all of them
				//
				// This should speed things up a teeny tiny bit
				glDrawElements(GL_TRIANGLES, quad_indices.size(), GL_UNSIGNED_INT, 0);
			}

			// We can probably assume that each rectangle shaped sprite is
			// equal to 4 vertices
			rendered_vertices += 4 * model_matrices.size();

			rendered_entities += model_matrices.size();
		}

		// Render text entities
		{
			PROFILER_SCOPE_RENDER("Render text")

			// The same VAO can be used for all text entities
			text_vao.bind();

			const auto view = entity_registry.view<birb::text>();

			for (const auto& ent : view)
			{
				// Skip inactive entities
				const state* state = entity_registry.try_get<birb::state>(ent);
				if (state && !state->active)
					continue;

				const birb::text& text = view.get<birb::text>(ent);

				// Skip the entity if the text component doens't have any text to render
				if (text.txt.empty())
					continue;

				// Skip the entity if its size makes it invisible
				if (text.scale == 0.0f)
					continue;

				// Fetch the shader
				std::shared_ptr<shader> shader = shader_collection::get_shader(text.shader);
				ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

				shader->set(shader_uniforms::text_color, text.color);
				shader->set(shader_uniforms::projection, projection_matrix);

				// We'll be drawing to TEXTURE0
				glActiveTexture(GL_TEXTURE0);

				f32 x = text.position.x;
				const f32 y = text.position.y;

				// Iterate through the text
				for (const char c : text.txt)
				{
					character& ch = text.font.get_char(c);

					vec2<f32> pos; // position
					pos.x = x + ch.bearing.x * text.scale;
					pos.y = y - (ch.size.y - ch.bearing.y) * text.scale;

					vec2<f32> dim; // dimensions
					dim.x = ch.size.x * text.scale;
					dim.y = ch.size.y * text.scale;

					// Update the VBO
					constexpr u8 vert_count = 6;

					const f32 verts[vert_count][4] = {
						{ pos.x,			pos.y + dim.y,	0.0f, 0.0f },
						{ pos.x,			pos.y,			0.0f, 1.0f },
						{ pos.x + dim.x, 	pos.y,			1.0f, 1.0f },

						{ pos.x,			pos.y + dim.y,	0.0f, 0.0f },
						{ pos.x + dim.x,	pos.y,			1.0f, 1.0f },
						{ pos.x + dim.x,	pos.y + dim.y,	1.0f, 0.0f }
					};

					glBindTexture(GL_TEXTURE_2D, ch.texture_id);

					glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					// We won't be using the draw_arrays() function since
					// we'll be using the same VAO for all of the text entities
					glDrawArrays(GL_TRIANGLES, 0, vert_count);

					// Move to the next char. One advance is 1/64 of a pixel
					// the bitshifting thing gets the value in pixels (2^6 = 64)
					// If you want to learn more about this function in general, check
					// this page where most of this code portion is
					// adapter from: https://learnopengl.com/In-Practice/Text-Rendering
					x += (ch.advance >> 6) * text.scale;

					rendered_vertices += vert_count;
				}

				text_vao.unbind();
				glBindTexture(GL_TEXTURE_2D, 0);

				++rendered_entities;
			}
		}

		/////////////////////
		// Post-processing //
		/////////////////////

		// Unbind the framebuffer object and draw the scene into a texture
		if (post_processing_enabled)
		{
			post_processing_fbo->unbind();

			std::shared_ptr<shader> post_shader = shader_collection::get_shader(post_processing_shader_ref);
			post_shader->activate();
			post_processing_fbo->frame_buffer_texture().bind();
			glDisable(GL_DEPTH_TEST);
			draw_elements(post_processing_vao, quad_indices.size());
			glEnable(GL_DEPTH_TEST);
			post_processing_fbo->frame_buffer_texture().unbind();
		}

		frame_id_counter++;

		/*****************************************************************************/
		/* Nothing will be rendered after this point if debug drawing is not enabled */
		/*****************************************************************************/

		// If debug drawing is not enabled, we can stop drawing stuff here
		if (!debug_view_enabled)
			return;

		///////////////////
		// Debug drawing //
		///////////////////

		// Draw the debug view
		debug.draw();

		// Draw box colliders
		{
			const std::shared_ptr<shader> debug_shader = shader_collection::get_shader(debug_shader_ref);
			debug_shader->set(shader_uniforms::view, projection_matrix);
			debug_shader->set(shader_uniforms::projection, projection_matrix);
			debug_shader->set(shader_uniforms::color, collider_debug_color);

			const auto view = entity_registry.view<collider::box>();
			for (const auto& entity : view)
			{
				const collider::box& box = view.get<collider::box>(entity);
				transform transform;
				transform.position = box.position();
				transform.local_scale = box.size();
				transform.local_scale.x += collider_cube_size_offset;
				transform.local_scale.y += collider_cube_size_offset;
				transform.local_scale.z += collider_cube_size_offset;

				debug_shader->set(shader_uniforms::model, transform.model_matrix());
				debug_shader->activate();

				draw_arrays(collider_debug_vao, cube_vertices.size());
			}
		}
	}

	void renderer::draw_elements(vao& vao, size_t index_count, gl_primitive primitive)
	{
		ensure(vao.id != 0);
		ensure(vao.contains_ebo());
		ensure(index_count > 0, "Unncessary call to draw_elements()");

		vao.bind();
		glDrawElements(static_cast<GLenum>(primitive), index_count, GL_UNSIGNED_INT, 0);
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive)
	{
		ensure(vao.id != 0);
		ensure(vert_count > 0, "Unncessary call to draw_arrays()");

#ifndef NDEBUG
		// TODO: This assert is a bit questionable and needs more attention
		// ensure(vao.d_total_vbo_vert_count == vert_count, "Something has gone wrong with VBO linking");
#endif

		vao.bind();
		glDrawArrays(static_cast<GLenum>(primitive), 0, vert_count);
	}

	void renderer::toggle_wireframe()
	{
		wireframe_mode = !wireframe_mode;

		if (!wireframe_mode)
		{
			birb::log("Toggling wireframe mode off");
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			birb::log("Toggling wireframe mode on");
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

	}

	bool renderer::is_wireframe_enabled()
	{
		return wireframe_mode;
	}

	void renderer::set_backface_culling(bool state)
	{
		if (state)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		backface_culling_enabled = state;
	}

	bool renderer::is_backface_culling_enabled()
	{
		return backface_culling_enabled;
	}

	void renderer::toggle_debug_view()
	{
		debug_view_enabled = !debug_view_enabled;

		if (!debug_view_enabled)
			birb::log("Toggling debug view off");
		else
			birb::log("Toggling debug view on");
	}

	u32 renderer::rendered_entities_count() const
	{
		return rendered_entities;
	}

	u32 renderer::rendered_vertex_count() const
	{
		return rendered_vertices;
	}

	void renderer::opt_blend(const bool enabled) const
	{
		ensure(opengl_initialized);

		if (enabled)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void renderer::opt_post_process(const bool enabled)
	{
		ensure(window != nullptr, "The window needs to be set before post-processing can be enabled");
		post_processing_enabled = enabled;
	}
}
