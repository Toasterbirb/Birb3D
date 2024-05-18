#include "Material.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "ShaderUniforms.hpp"
#include "State.hpp"
#include "Transform.hpp"

namespace birb
{
	void renderer::draw_3d_entities(const glm::mat4& view_matrix, const glm::mat4& perspective_projection_matrix)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		// Render all models
		{
			PROFILER_SCOPE_RENDER("Render models");

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
					shader->set(shader_uniforms::projection, perspective_projection_matrix);

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
				++stat_3d.entities;
				stat_3d.vertices += view.get<birb::model>(ent).vertex_count();
			}
		}
	}
}
