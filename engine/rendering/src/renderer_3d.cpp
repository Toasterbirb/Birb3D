#include "BoxCollider.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "ShaderUniforms.hpp"
#include "State.hpp"
#include "Transform.hpp"

namespace birb
{
	void renderer::draw_3d_entities(const glm::mat4& view_matrix, const glm::mat4& perspective_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_models(view_matrix, perspective_projection);

		// Box collider visualization
		if (debug_view_enabled)
			draw_box_collider_view(view_matrix, perspective_projection);
	}

	void renderer::draw_models(const glm::mat4& view_matrix, const glm::mat4& perspective_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

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
				shader->set(shader_uniforms::projection, perspective_projection);

				shader->update_directional_light();
				shader->update_point_lights();
				uniforms_updated.insert(shader->id);
			}

			shader->set(shader_uniforms::model, transform.model_matrix());

			// Apply the material component on the shader if it has any
			// TODO: Make this work with textures too
			bool skip_mesh_materials = false;
			const birb::material* material = entity_registry.try_get<birb::material>(ent);
			if (material != nullptr)
			{
				shader->apply_color_material(*material);
				skip_mesh_materials = true;
			}

			// Draw the model
			ensure(view.get<birb::model>(ent).vertex_count() != 0, "Tried to render a model with no vertices");
			view.get<birb::model>(ent).draw(*shader, skip_mesh_materials);
			++render_stats.entities_3d;
			render_stats.vertices_3d += view.get<birb::model>(ent).vertex_count();
		}
	}

	void renderer::draw_box_collider_view(const glm::mat4& view_matrix, const glm::mat4& perspective_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const std::shared_ptr<shader> debug_shader = shader_collection::get_shader(debug_shader_ref);
		debug_shader->set(shader_uniforms::view, view_matrix);
		debug_shader->set(shader_uniforms::projection, perspective_projection);
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
