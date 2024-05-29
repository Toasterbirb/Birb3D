#include "Line.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "State.hpp"
#include "Text.hpp"

namespace birb
{
	void renderer::draw_screenspace_entities()
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_lines();
		draw_text();
	}

	void renderer::draw_lines()
	{
		PROFILER_SCOPE_RENDER_FN();

		const entt::registry& entity_registry = current_scene->registry;

		// Get the shader for rendering lines
		std::shared_ptr<birb::shader> shader = shader_collection::get_shader(line_shader);

		const auto view = entity_registry.view<birb::line>();

		for (const auto& ent : view)
		{
			// Skip inactive entities
			const state* state = entity_registry.try_get<birb::state>(ent);
			if (state && !state->active)
				continue;

			const birb::line& line = view.get<birb::line>(ent);
			line.vao->bind();

			shader->set(shader_uniforms::color, line.color);

			// Draw the line
			draw_arrays(2, gl_primitive::lines);
			line.vao->unbind();
		}
	}

	void renderer::draw_text()
	{
		PROFILER_SCOPE_RENDER_FN();

		const entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<birb::text>();

		shader_ref text_atlas_shader("text_atlas", "text_atlas");
		std::shared_ptr<birb::shader> shader = shader_collection::get_shader(text_atlas_shader);
		shader->activate();
		shader->set(shader_uniforms::text::atlas, 0);

		glActiveTexture(GL_TEXTURE0);

		glDisable(GL_DEPTH_TEST);
		set_backface_culling(false);

		const f32 fullscreen_verts[6 * 4] = {
			-1.0f,	 1.0f,	0.0f, 0.0f,
			-1.0f,	-1.0f,	0.0f, 1.0f,
			 1.0f, 	-1.0f,	1.0f, 1.0f,

			-1.0f,	 1.0f,	0.0f, 0.0f,
			 1.0f,	-1.0f,	1.0f, 1.0f,
			 1.0f,	 1.0f,	1.0f, 0.0f
		};

		u32 vao, vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreen_verts), &fullscreen_verts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		for (const auto& entity : view)
		{
			// Check if the entity should be skipped because its not active
			if (!current_scene->is_entity_active(entity))
				continue;

			const birb::text& text = view.get<birb::text>(entity);

			// Skip the entity if the text component doens't have any text to render
			if (text.empty())
				continue;

			shader->activate();
			// shader->set(shader_uniforms::text::position, text.position);

			// const vec2<f32> dim = { 4, 4 };
			glBindVertexArray(vao);

			// Avoid const trouble and do the frame buffer binding manually
			glBindTexture(GL_TEXTURE_2D, text.atlas_id());
			draw_arrays(6, gl_primitive::triangles);
			glBindTexture(GL_TEXTURE_2D, 0);

			// glBindVertexArray(0);

			++render_stats.entities_screenspace;
			render_stats.vertices_screenspace += 4; // 4 vertices per quad
		}

		glEnable(GL_DEPTH_TEST);

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		post_processing_vao.unbind();
	}
}
