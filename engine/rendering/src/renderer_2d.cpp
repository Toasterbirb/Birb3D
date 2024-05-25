#include "Box2DCollider.hpp"
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

namespace birb
{
	void renderer::draw_2d_entities()
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_sprites();
		draw_sprites_instanced();
	}

	void renderer::draw_sprites()
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<sprite, transform>();
		const u32 sprite_count = std::distance(view.begin(), view.end());

		// We don't need to do anything if there are no sprites to render
		if (sprite_count == 0)
			return;


		const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(texture_shader_ref);

		// Sprites should only have a singular texture, so we'll use the default
		// tex0 texture unit
		texture_shader->set(shader_uniforms::texture_units::tex0, 0);

		texture_shader->set(shader_uniforms::texture::instanced, 0);
		sprite_vao.bind();


		// Calculate model matrices in parallel
		std::vector<glm::mat4> model_matrices(sprite_count);

		{
			PROFILER_SCOPE_RENDER("Calculate transform model matrices");

			std::transform(std::execution::par, view.begin(), view.end(), model_matrices.begin(),
				[view](const auto& entity)
				{
					return view.get<birb::transform>(entity).model_matrix();
				}
			);
		}

		size_t sprite_index = 0;
		texture_shader->activate();
		sprite_vao.bind();

		u32 previous_texture_id = 0;

		for (const auto& ent : view)
		{
			// Don't render entities that are inactive
			const birb::state* state = entity_registry.try_get<birb::state>(ent);
			if (state && !state->active)
				continue;

			sprite& entity_sprite = view.get<birb::sprite>(ent);

			texture_shader->set(shader_uniforms::model, model_matrices[sprite_index++]);
			set_sprite_aspect_ratio_uniforms(entity_sprite, *texture_shader);

			if (entity_sprite.texture->id != previous_texture_id)
			{
				entity_sprite.texture->bind();
				previous_texture_id = entity_sprite.texture->id;
			}

			draw_elements(quad_indices.size());

			// We can probably assume that each rectangle shaped sprite is
			// equal to 4 vertices
			render_stats.vertices_2d += 4;
			++render_stats.entities_2d;
		}

	}

	void renderer::draw_sprites_instanced()
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<sprite, transformer>();
		const u32 sprite_count = std::distance(view.begin(), view.end());

		// We don't need to do anything if there are no instanced sprites to render
		if (sprite_count == 0)
			return;

		const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(texture_shader_ref);

		// Sprites should only have a singular texture, so we'll use the default
		// tex0 texture unit
		texture_shader->set(shader_uniforms::texture_units::tex0, 0);

		texture_shader->set(shader_uniforms::texture::instanced, 1);
		sprite_vao.bind();

		texture_shader->activate();
		sprite_vao.bind();

		constexpr u8 first_layout_index = 5;
		constexpr u8 vec4_component_count = 4;
		constexpr size_t vec4_size = sizeof(glm::vec4);

		// Enable the vertex attrib arrays
		for (u8 i = 0; i < vec4_component_count; ++i)
		{
			glEnableVertexAttribArray(first_layout_index + i);
			glVertexAttribDivisor(first_layout_index + i, 1);
		}

		for (const auto& ent : view)
		{
			// Don't render entities that are inactive
			const birb::state* state = entity_registry.try_get<birb::state>(ent);
			if (state && !state->active)
				continue;

			const sprite& sprite = view.get<birb::sprite>(ent);
			const transformer& transformer = view.get<birb::transformer>(ent);
			ensure(transformer.is_locked(), "Using an unlocked transformer is very inefficient");

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

		sprite_vao.unbind();
	}
}
