#include "Box2DCollider.hpp"
#include "Camera.hpp"
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
	void renderer::draw_2d_entities(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection, const camera& camera, const vec2<i32> window_size)
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_sprites(view_matrix, orthographic_projection, camera, window_size);
	}

	void renderer::draw_sprites(const glm::mat4& view_matrix, const glm::mat4& orthographic_projection, const camera& camera, const vec2<i32> window_size)
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

		// Calculate the screen area position and dimensions for culling purposes
		transform screen_area_transform;
		screen_area_transform.position.x = camera.position.x + (window_size.x * 0.5f) * camera.orthograhpic_scale;
		screen_area_transform.position.y = camera.position.y + (window_size.y * 0.5f) * camera.orthograhpic_scale;
		screen_area_transform.local_scale.x = window_size.x * camera.orthograhpic_scale;
		screen_area_transform.local_scale.y = window_size.y * camera.orthograhpic_scale;
		const collider::box2d screen_area(screen_area_transform);


		const auto view = entity_registry.view<sprite, transform>();
		const u32 sprite_count = std::distance(view.begin(), view.end());

		// false = the sprite has a calculated model matrix and it should be drawn
		// true = the sprite is culled and shouldn't be drawn
		std::vector<bool> culling_list(sprite_count);

		{
			PROFILER_SCOPE_RENDER("Sprite culling");

			std::transform(std::execution::par, view.begin(), view.end(), culling_list.begin(),
				[view, screen_area, camera](const auto& entity)
				{
					sprite& entity_sprite = view.get<birb::sprite>(entity);
					transform entity_transform = view.get<birb::transform>(entity);
					entity_transform.local_scale * camera.orthograhpic_scale;

					// Check if the sprite is visible in the viewport
					collider::box2d sprite_collider(entity_transform);
					if (screen_area.collides_with(sprite_collider))
						return true;
					else
						return false;
				}
			);
		}


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
			// Check if the sprite should be skipped
			if (!culling_list[sprite_index])
			{
				++sprite_index;
				continue;
			}

			texture_shader->set(shader_uniforms::model, model_matrices[sprite_index++]);

			sprite& entity_sprite = view.get<birb::sprite>(ent);

			if (entity_sprite.ignore_aspect_ratio)
				texture_shader->set(shader_uniforms::texture::aspect_ratio, { 1.0f, 1.0f });
			else
				// Modify the sprite shape based on if we want to respect the aspect ratio width or height wise
				if (entity_sprite.aspect_ratio_lock == sprite::aspect_ratio_lock::width)
					texture_shader->set(shader_uniforms::texture::aspect_ratio, { entity_sprite.texture->aspect_ratio(), 1.0f });
				else
					texture_shader->set(shader_uniforms::texture::aspect_ratio, { 1.0f, entity_sprite.texture->aspect_ratio_reverse() });

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
}
