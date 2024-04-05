#include "Globals.hpp"
#include "Logger.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "Sprite.hpp"
#include "State.hpp"
#include "Transform.hpp"

// Debug drawing dependencies
#include "BoxCollider.hpp"

#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <entt.hpp>
#include <glad/gl.h>
#include <memory>
#include <unordered_set>

#define TEXTURE_SHADER_NAME "texture"

// Make sure that all of the datatypes are of correct size
// so that they work correctly with OpenGL
static_assert(sizeof(double) == sizeof(GLdouble));
static_assert(sizeof(float) == sizeof(GLfloat));
static_assert(sizeof(int) == sizeof(GLint));
static_assert(sizeof(long int) == sizeof(GLint64));
static_assert(sizeof(short) == sizeof(GLshort));
static_assert(sizeof(std::byte) == sizeof(GLbyte));
static_assert(sizeof(unsigned int) == sizeof(GLuint));
static_assert(sizeof(unsigned long int) == sizeof(GLuint64));
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
		assert(scene::scene_count() > 0);
		current_scene = &scene;
	}

	void renderer::set_window(birb::window& window)
	{
		this->window = &window;
	}

	void renderer::draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(current_scene != nullptr);
		assert(scene::scene_count() > 0);
		assert(!buffers_flipped && "Tried to draw entities after the buffers were already flipped");

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

		// Store shaders that have some specific uniforms updated already
		// to avoid duplicate uniform updates
		std::unordered_set<u32> uniforms_updated;

		// Render all models
		{
			PROFILER_SCOPE_RENDER("Render models")

			const auto view = entity_registry.view<birb::model, birb::shader_ref, birb::component::transform>();
			for (const auto& ent : view)
			{
				// Check if the entity should be skipped because its not active
				const component::state* state = entity_registry.try_get<component::state>(ent);
				if (state && !state->active)
					continue;

				// Get the shader we'll be using for drawing the meshes of the model
				shader_ref& shader_reference = view.get<birb::shader_ref>(ent);
				std::shared_ptr<shader> shader = shader_collection::get_shader(shader_reference.vertex, shader_reference.fragment);

				assert(shader->id != 0 && "Tried to use an invalid shader for rendering");

				const birb::component::transform& transform = view.get<birb::component::transform>(ent);

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
				const birb::component::material* material = entity_registry.try_get<birb::component::material>(ent);
				if (material != nullptr)
					shader->apply_color_material(*material);

				// Draw the model
				assert(view.get<birb::model>(ent).vertex_count() != 0 && "Tried to render a model with no vertices");
				view.get<birb::model>(ent).draw(*shader);
				++rendered_entities;
				rendered_vertices += view.get<birb::model>(ent).vertex_count();
			}
		}

		// Render sprites
		{
			PROFILER_SCOPE_RENDER("Render sprites")

			const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(TEXTURE_SHADER_NAME, TEXTURE_SHADER_NAME);

			// Sprites should only have a singular texture, so we'll use the default
			// tex0 texture unit
			texture_shader->set(shader_uniforms::texture_units::tex0, 0);

			texture_shader->set(shader_uniforms::view, view_matrix);
			texture_shader->set(shader_uniforms::projection, projection_matrix);
			sprite_vao.bind();


			const auto view = entity_registry.view<sprite, component::transform>();

			// Calculate model matrices in parallel
			std::vector<glm::mat4> model_matrices(std::distance(view.begin(), view.end()));

			{
				PROFILER_SCOPE_RENDER("Calculate transform model matrices")

				std::transform(std::execution::par, view.begin(), view.end(), model_matrices.begin(), [view](const auto& entity) {
					return view.get<birb::component::transform>(entity).model_matrix();
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


		/////////////////////
		// Post-processing //
		/////////////////////

		// Unbind the framebuffer object and draw the scene into a texture
		if (post_processing_enabled)
		{
			post_processing_fbo->unbind();

			std::shared_ptr<shader> post_shader = shader_collection::get_shader("post_process", "post_process");
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

		// Draw box colliders
		{
			const std::shared_ptr<shader> debug_shader = shader_collection::get_shader("color", "color");
			debug_shader->set(shader_uniforms::view, projection_matrix);
			debug_shader->set(shader_uniforms::projection, projection_matrix);
			debug_shader->set(shader_uniforms::color, collider_debug_color);

			const auto view = entity_registry.view<collider::box>();
			for (const auto& entity : view)
			{
				const collider::box& box = view.get<collider::box>(entity);
				component::transform transform;
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
		assert(vao.id != 0);
		assert(vao.contains_ebo());
		assert(index_count > 0 && "Unncessary call to draw_elements()");

		vao.bind();
		glDrawElements(static_cast<GLenum>(primitive), index_count, GL_UNSIGNED_INT, 0);
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive)
	{
		assert(vao.id != 0);
		assert(vert_count > 0 && "Unncessary call to draw_arrays()");

#ifndef NDEBUG
		assert(vao.d_total_vbo_vert_count == vert_count && "Something has gone wrong with VBO linking");
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
		assert(opengl_initialized);

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
		assert(window != nullptr && "The window needs to be set before post-processing can be enabled");
		post_processing_enabled = enabled;
	}
}
