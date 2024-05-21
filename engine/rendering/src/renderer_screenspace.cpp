#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "State.hpp"
#include "Text.hpp"

namespace birb
{
	void renderer::draw_screenspace_entities(const glm::mat4& orthographic_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_text(orthographic_projection);
	}

	void renderer::draw_text(const glm::mat4& orthographic_projection)
	{
		PROFILER_SCOPE_RENDER_FN();

		const entt::registry& entity_registry = current_scene->registry;

		const auto view = entity_registry.view<birb::text>();

		for (const auto& ent : view)
		{
			// Skip inactive entities
			const state* state = entity_registry.try_get<birb::state>(ent);
			if (state && !state->active)
				continue;

			const birb::text& text = view.get<birb::text>(ent);

			// Skip the entity if the text component doens't have any text to render
			if (text.empty())
				continue;

			// Skip the entity if its size makes it invisible
			if (text.scale == 0.0f)
				continue;

			// Fetch the shader
			const std::shared_ptr<shader> shader = shader_collection::get_shader(text.shader);
			ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

			shader->set(shader_uniforms::text_color, text.color);
			shader->set(shader_uniforms::projection, orthographic_projection);

			// The same VAO can be used for all characters
			text_vao.bind();

			// We'll be drawing to TEXTURE0
			glActiveTexture(GL_TEXTURE0);

			const std::set<char>& chars = text.chars();

			// Iterate through the text
			for (const char c : chars)
			{
				// Update the VBO
				constexpr u8 vert_count = 6;

				const vec2<f32>& dim = text.char_dimensions(c);

				const f32 verts[vert_count][4] = {
					{ 0,		dim.y,	0.0f, 0.0f },
					{ 0,		0,		0.0f, 1.0f },
					{ dim.x, 	0,		1.0f, 1.0f },

					{ 0,		dim.y,	0.0f, 0.0f },
					{ dim.x,	0, 		1.0f, 1.0f },
					{ dim.x,	dim.y,	1.0f, 0.0f }
				};

				glBindTexture(GL_TEXTURE_2D, text.char_texture_id(c));

				glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), nullptr);

				glBindBuffer(GL_ARRAY_BUFFER, text.instance_vbo(c));
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), nullptr);
				glEnableVertexAttribArray(1);
				glVertexAttribDivisor(1, 1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);


				// draw_arrays(vert_count);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, text.char_positions(c).size());
				++render_stats.draw_arrays_instanced;


				render_stats.vertices_screenspace += vert_count * text.char_positions(c).size();
			}

			text_vao.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);

			++render_stats.entities_screenspace;
		}

	}
}
