#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "Sprite.hpp"
#include "Transform.hpp"

#include <algorithm>
#include <execution>

namespace birb
{
	void renderer::draw_2d_entities(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_sprites(view_matrix, orthographic_projection);
	}

	void renderer::draw_sprites(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const std::shared_ptr<shader> texture_shader = shader_collection::get_shader(texture_shader_ref);

		// Sprites should only have a singular texture, so we'll use the default
		// tex0 texture unit
		texture_shader->set(shader_uniforms::texture_units::tex0, 0);

		texture_shader->set(shader_uniforms::view, view_matrix);
		texture_shader->set(shader_uniforms::projection, orthographic_projection);
		sprite_vao.bind();


		const auto view = entity_registry.view<sprite, transform>();

		// Calculate model matrices in parallel
		std::vector<glm::mat4> model_matrices(std::distance(view.begin(), view.end()));

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
					texture_shader->set(shader_uniforms::texture::aspect_ratio_reverse,
							entity_sprite.texture->aspect_ratio_reverse());
				}
			}

			entity_sprite.texture->bind();

			draw_elements(quad_indices.size());
		}

		// We can probably assume that each rectangle shaped sprite is
		// equal to 4 vertices
		render_stats.vertices_2d += 4 * model_matrices.size();

		render_stats.entities_2d += model_matrices.size();
	}
}
