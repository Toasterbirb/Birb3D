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

#include <algorithm>
#include <execution>
#include <vector>

namespace birb
{
	void renderer::draw_3d_entities(const glm::mat4& view_matrix, const glm::mat4& perspective_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_models();

		// Box collider visualization
		if (debug_view_enabled)
			draw_box_collider_view(view_matrix, perspective_projection);
	}

	void renderer::draw_models()
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		// Store shaders that have some specific uniforms updated already
		// to avoid duplicate uniform updates
		std::unordered_set<u32> uniforms_updated;

		const auto view = entity_registry.view<birb::model, birb::shader_ref, birb::transform>();

		// Process some data for each entity in parallel with std::transform

		struct model_data
		{
			bool is_active = true;
			birb::model* model = nullptr;
			glm::mat4 model_matrix;
			shader_ref* shader = nullptr;
			birb::material* material = nullptr;
		};

		std::vector<model_data> model_data_array(std::distance(view.begin(), view.end()));

		{
			PROFILER_SCOPE_RENDER("Process transform model matrices for 3D models");

			std::transform(std::execution::par, view.begin(), view.end(), model_data_array.begin(),
				[view, &entity_registry](auto& entity)
				{
					model_data data;
					data.is_active = true;

					birb::state* state = entity_registry.try_get<birb::state>(entity);
					if (state)
						data.is_active = state->active;

					// Don't do any unnecessary work if the entity is not active
					// This makes the assumption that the rendering loop doesn't touch
					// the rest of the variables if the entity is not active
					if (!data.is_active)
						return data;

					data.model = &view.get<birb::model>(entity);
					data.model_matrix = view.get<birb::transform>(entity).model_matrix();
					data.shader = &view.get<birb::shader_ref>(entity);
					data.material = entity_registry.try_get<birb::material>(entity);

					return data;
				}
			);
		}


		for (const model_data& data : model_data_array)
		{
			// Check if the entity should be skipped because its not active
			if (!data.is_active)
				continue;

			// Get the shader we'll be using for drawing the meshes of the model
			std::shared_ptr<shader> shader = shader_collection::get_shader(*data.shader);
			shader->activate();

			ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

			// Make sure the lighting is up-to-date
			if (!uniforms_updated.contains(shader->id))
			{
				shader->update_directional_light();
				shader->update_point_lights();
				uniforms_updated.insert(shader->id);
			}

			shader->set(shader_uniforms::model, data.model_matrix);

			// Apply the material component on the shader if it has any
			// TODO: Make this work with textures too
			bool skip_mesh_materials = false;
			if (data.material != nullptr)
			{
				shader->apply_color_material(*data.material);
				skip_mesh_materials = true;
			}

			// Draw the model
			ensure(data.model->vertex_count() != 0, "Tried to render a model with no vertices");
			data.model->draw(*shader, render_stats, skip_mesh_materials);
			++render_stats.entities_3d;
			render_stats.vertices_3d += data.model->vertex_count();
		}
	}

	void renderer::draw_box_collider_view(const glm::mat4& view_matrix, const glm::mat4& perspective_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<collider::box>();

		// Don't do anything if there are no box colliders
		if (std::distance(view.begin(), view.end()) == 0)
			return;

		bool backface_culling_enabled = is_backface_culling_enabled();
		if (backface_culling_enabled)
			set_backface_culling(false);

		const std::shared_ptr<shader> debug_shader = shader_collection::get_shader(debug_shader_ref);
		debug_shader->set(shader_uniforms::view, view_matrix);
		debug_shader->set(shader_uniforms::projection, perspective_projection);
		debug_shader->set(shader_uniforms::color, collider_debug_color);

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

		if (backface_culling_enabled)
			set_backface_culling(true);
	}
}
