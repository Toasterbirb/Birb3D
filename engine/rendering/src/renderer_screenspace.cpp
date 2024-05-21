#include "Character.hpp"
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

			std::shared_ptr<std::vector<text::char_data>> chars = text.chars();

			// The characters are sorted, so we can avoid unnecessary texture
			// changes by only binding a new texture when the ID changes
			char last_char = 0;

			// Iterate through the text
			for (text::char_data c : *chars)
			{
				// Update the VBO
				constexpr u8 vert_count = 6;

				const f32 verts[vert_count][4] = {
					{ c.pos.x,				c.pos.y + c.dim.y,	0.0f, 0.0f },
					{ c.pos.x,				c.pos.y,			0.0f, 1.0f },
					{ c.pos.x + c.dim.x, 	c.pos.y,			1.0f, 1.0f },

					{ c.pos.x,				c.pos.y + c.dim.y,	0.0f, 0.0f },
					{ c.pos.x + c.dim.x,	c.pos.y,			1.0f, 1.0f },
					{ c.pos.x + c.dim.x,	c.pos.y + c.dim.y,	1.0f, 0.0f }
				};

				if (c.c != last_char)
				{
					glBindTexture(GL_TEXTURE_2D, c.texture_id);
					last_char = c.c;
				}

				glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				draw_arrays(vert_count);

				render_stats.vertices_screenspace += vert_count;
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			text_vao.unbind();

			++render_stats.entities_screenspace;
		}

	}
}
