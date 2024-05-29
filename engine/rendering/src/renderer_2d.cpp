#include "Box2DCollider.hpp"
#include "MimicSprite.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "Sprite.hpp"
#include "State.hpp"
#include "Transform.hpp"
#include "Transformer.hpp"

#include <algorithm>
#include <execution>
#include <glad/gl.h>

namespace birb
{
	void renderer::draw_2d_entities()
	{
		PROFILER_SCOPE_RENDER_FN();

		// Use the same VAO for all sprites
		sprite_vao.bind();

		// Sprites should only have a singular texture, so we'll use the default
		// tex0 texture unit
		const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(texture_shader_ref);
		texture_shader->activate();
		texture_shader->set(shader_uniforms::texture_units::tex0, 0);

		texture_shader->set(shader_uniforms::texture::instanced, 0);
		draw_sprites(texture_shader);
		draw_mimic_sprites(texture_shader);

		texture_shader->set(shader_uniforms::texture::instanced, 1);
		draw_sprites_instanced(texture_shader);

		sprite_vao.unbind();
	}

	void renderer::draw_sprites(std::shared_ptr<shader> texture_shader)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<sprite, transform>();
		const u32 sprite_count = std::distance(view.begin(), view.end());

		// We don't need to do anything if there are no sprites to render
		if (sprite_count == 0)
			return;

		// Calculate model matrices in parallel and pair the model matrices
		// with the entities

		struct sprite_data
		{
			bool is_active;
			birb::sprite* sprite;
			glm::mat4 model_matrix;
		};

		std::vector<sprite_data> sprite_model_array(sprite_count);

		{
			PROFILER_SCOPE_RENDER("Calculate transform model matrices");

			std::transform(std::execution::par, view.begin(), view.end(), sprite_model_array.begin(),
				[view, &entity_registry](auto& entity)
				{
					sprite_data data;
					data.is_active = true;

					birb::state* state = entity_registry.try_get<birb::state>(entity);
					if (state)
						data.is_active = state->active;

					// Avoid unnecessary work if the entity is disabled
					// This expects that the rendering loop won't touch any of the missing variables
					// if the entity is disabled
					if (!data.is_active)
						return data;

					data.sprite = &view.get<birb::sprite>(entity);
					data.model_matrix = view.get<birb::transform>(entity).model_matrix();

					return data;
				}
			);
		}


		for (const sprite_data& data : sprite_model_array)
		{
			// Don't render entities that are inactive
			if (!data.is_active)
				continue;

			texture_shader->set(shader_uniforms::model, data.model_matrix);
			texture_shader->set(shader_uniforms::texture::orthographic, data.sprite->orthographic_projection);
			set_sprite_aspect_ratio_uniforms(*data.sprite, *texture_shader);

			data.sprite->texture->bind();

			draw_elements(quad_indices.size());

			// We can probably assume that each rectangle shaped sprite is
			// equal to 4 vertices
			render_stats.vertices_2d += 4;
			++render_stats.entities_2d;
		}

	}

	void renderer::draw_sprites_instanced(std::shared_ptr<shader> texture_shader)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<sprite, transformer>();
		const u32 sprite_count = std::distance(view.begin(), view.end());

		// We don't need to do anything if there are no instanced sprites to render
		if (sprite_count == 0)
			return;

		constexpr u8 first_layout_index = 5;
		constexpr u8 vec4_component_count = 4;
		constexpr size_t vec4_size = sizeof(glm::vec4);

		// Enable the vertex attrib arrays
		for (u8 i = 0; i < vec4_component_count; ++i)
		{
			glEnableVertexAttribArray(first_layout_index + i);
			glVertexAttribDivisor(first_layout_index + i, 1);
		}

		for (const auto& entity : view)
		{
			// Don't render entities that are inactive
			if (!current_scene->is_entity_active(entity))
				continue;

			const sprite& sprite = view.get<birb::sprite>(entity);
			const transformer& transformer = view.get<birb::transformer>(entity);
			ensure(transformer.is_locked(), "Using an unlocked transformer is very inefficient");

			texture_shader->set(shader_uniforms::texture::orthographic, sprite.orthographic_projection);

			set_sprite_aspect_ratio_uniforms(sprite, *texture_shader);
			sprite.texture->bind();
			transformer.bind_vbo();

			for (u8 i = 0; i < vec4_component_count; ++i)
				glVertexAttribPointer(first_layout_index + i, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, reinterpret_cast<void*>(i * vec4_size));

			draw_elements_instanced(quad_indices.size(), transformer.transforms.size());
			++render_stats.draw_elements_instanced;
			render_stats.vertices_2d += 4 * transformer.transforms.size();
			render_stats.entities_2d += transformer.transforms.size();
		}

		// Disable the vertex attrib arrays
		for (u8 i = 0; i < vec4_component_count; ++i)
			glDisableVertexAttribArray(first_layout_index + i);

		vbo::unbind();
	}

	void renderer::draw_mimic_sprites(std::shared_ptr<shader> texture_shader)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<mimic_sprite, transform>();

		for (const auto& entity : view)
		{
			// Don't render entities that are inactive
			if (!current_scene->is_entity_active(entity))
				continue;

			const mimic_sprite& entity_sprite = view.get<birb::mimic_sprite>(entity);
			const birb::transform& transform = view.get<birb::transform>(entity);

			texture_shader->set(shader_uniforms::model, transform.model_matrix());
			texture_shader->set(shader_uniforms::texture::orthographic, entity_sprite.orthographic_projection);
			set_sprite_aspect_ratio_uniforms(entity_sprite, *texture_shader);

			entity_sprite.texture->bind();

			draw_elements(quad_indices.size());

			// We can probably assume that each rectangle shaped sprite is
			// equal to 4 vertices
			render_stats.vertices_2d += 4;
			++render_stats.entities_2d;
		}
	}
}
