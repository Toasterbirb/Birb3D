#include "Logger.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "Sprite.hpp"
#include "Transform.hpp"
#include "glm/fwd.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <entt.hpp>
#include <glad/gl.h>
#include <memory>

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

namespace birb
{
	renderer::renderer()
	{
		event_bus::register_event_id(event::toggle_wireframe_rendering_mode, this);

		// Initialize the opengl buffers for sprite rendering
		sprite_vao.bind();

		sprite_vbo = std::make_shared<vbo>(square_vertices);
		sprite_ebo = std::make_shared<ebo>(square_indices);

		constexpr unsigned int vertex_pos_layout = 0;
		constexpr unsigned int texture_coord_layout = 1;

		constexpr unsigned int vertex_component_count = 3;
		constexpr unsigned int texture_coord_component_count = 2;

		constexpr unsigned int vertex_offset = 0;
		constexpr unsigned int texture_coord_offset = vertex_component_count;

		constexpr unsigned int stride = 5;

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
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
		event_bus::unregister_event_id(event::toggle_wireframe_rendering_mode, this);
	}

	void renderer::process_event(unsigned short event_id, const event_data& data)
	{
		switch (event_id)
		{
			case 1:
				toggle_wireframe();
				break;
		}
	}

	void renderer::set_scene(scene& scene)
	{
		assert(scene::scene_count() > 0);
		current_scene = &scene;
	}

	void renderer::draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(current_scene != nullptr);
		assert(scene::scene_count() > 0);

		entt::registry& entity_registry = current_scene->registry;

		rendered_entities = 0;
		rendered_vertices = 0;

		// Render all models
		{
			const auto view = entity_registry.view<birb::model, birb::shader, birb::component::transform>();
			for (const auto& ent : view)
			{
				// Get the shader we'll be using for drawing the meshes of the model
				shader& shader = view.get<birb::shader>(ent);
				assert(shader.id != 0 && "Tried to use an invalid shader for rendering");

				const birb::component::transform& transform = view.get<birb::component::transform>(ent);

				shader.set(shader_uniforms::model, transform.model_matrix());
				shader.set(shader_uniforms::view, view_matrix);
				shader.set(shader_uniforms::projection, projection_matrix);

				// Make sure the lighting is up-to-date
				shader.update_directional_light();
				shader.update_point_lights();

				// Apply the material component on the shader if it has any
				// TODO: Make this work with textures too
				const birb::component::material* material = entity_registry.try_get<birb::component::material>(ent);
				if (material != nullptr)
					shader.apply_color_material(*material);

				// Draw the model
				assert(view.get<birb::model>(ent).vertex_count() != 0 && "Tried to render a model with no vertices");
				view.get<birb::model>(ent).draw(shader);
				++rendered_entities;
				rendered_vertices += view.get<birb::model>(ent).vertex_count();
			}
		}

		// Render sprites
		{
			const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(TEXTURE_SHADER_NAME, TEXTURE_SHADER_NAME);

			// Sprites should only have a singular texture, so we'll use the default
			// tex0 texture unit
			texture_shader->set(shader_uniforms::texture_units::tex0, 0);

			texture_shader->set(shader_uniforms::view, view_matrix);
			texture_shader->set(shader_uniforms::projection, projection_matrix);


			const auto view = entity_registry.view<sprite, component::transform>();
			for (const auto& ent : view)
			{
				// Update the model matrix and activate the shader
				const component::transform& transform = view.get<birb::component::transform>(ent);
				texture_shader->set(shader_uniforms::model, transform.model_matrix());
				texture_shader->activate();

				std::shared_ptr<texture> texture = view.get<sprite>(ent).texture;

				sprite_vao.bind();
				texture->bind();
				draw_elements(sprite_vao, square_indices.size());

				++rendered_entities;

				// We can probably assume that each rectangle shaped sprite is
				// equal to 4 vertices
				rendered_vertices += 4;
			}
		}
	}

	void renderer::draw_elements(vao& vao, size_t index_count)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(vao.id != 0);
		assert(index_count > 0 && "Unncessary call to draw_elements()");

		vao.bind();
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(vao.id != 0);
		assert(vert_count > 0 && "Unncessary call to draw_arrays()");

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vert_count);
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

	unsigned int renderer::rendered_entities_count() const
	{
		return rendered_entities;
	}

	unsigned int renderer::rendered_vertex_count() const
	{
		return rendered_vertices;
	}
}
