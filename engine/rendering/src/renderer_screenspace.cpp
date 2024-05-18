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

		// The same VAO can be used for all text entities
		text_vao.bind();

		const auto view = entity_registry.view<birb::text>();

		for (const auto& ent : view)
		{
			// Skip inactive entities
			const state* state = entity_registry.try_get<birb::state>(ent);
			if (state && !state->active)
				continue;

			const birb::text& text = view.get<birb::text>(ent);

			// Skip the entity if the text component doens't have any text to render
			if (text.txt.empty())
				continue;

			// Skip the entity if its size makes it invisible
			if (text.scale == 0.0f)
				continue;

			// Fetch the shader
			const std::shared_ptr<shader> shader = shader_collection::get_shader(text.shader);
			ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

			shader->set(shader_uniforms::text_color, text.color);
			shader->set(shader_uniforms::projection, orthographic_projection);

			// We'll be drawing to TEXTURE0
			glActiveTexture(GL_TEXTURE0);

			f32 x = text.position.x;
			f32 y = text.position.y;

			// Iterate through the text
			for (const char c : text.txt)
			{
				// Process newline characters
				if (c == '\n')
				{
					// Go back to the starting position width wise
					x = text.position.x;

					// Go one line down
					y -= text.font.size() * text.scale;

					// We shouldn't draw the newline char
					continue;
				}

				const character& ch = text.font.get_char(c);

				// position
				const vec2<f32> pos(
					x + ch.bearing.x * text.scale,
					y - (ch.size.y - ch.bearing.y) * text.scale
				);

				// dimensions
				const vec2<f32> dim(
					ch.size.x * text.scale,
					ch.size.y * text.scale
				);

				// Update the VBO
				constexpr u8 vert_count = 6;

				const f32 verts[vert_count][4] = {
					{ pos.x,			pos.y + dim.y,	0.0f, 0.0f },
					{ pos.x,			pos.y,			0.0f, 1.0f },
					{ pos.x + dim.x, 	pos.y,			1.0f, 1.0f },

					{ pos.x,			pos.y + dim.y,	0.0f, 0.0f },
					{ pos.x + dim.x,	pos.y,			1.0f, 1.0f },
					{ pos.x + dim.x,	pos.y + dim.y,	1.0f, 0.0f }
				};

				glBindTexture(GL_TEXTURE_2D, ch.texture_id);

				glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				// We won't be using the draw_arrays() function since
				// we'll be using the same VAO for all of the text entities
				glDrawArrays(GL_TRIANGLES, 0, vert_count);

				// Move to the next char. One advance is 1/64 of a pixel
				// the bitshifting thing gets the value in pixels (2^6 = 64)
				// If you want to learn more about this function in general, check
				// this page where most of this code portion is
				// adapter from: https://learnopengl.com/In-Practice/Text-Rendering
				x += (ch.advance >> 6) * text.scale;

				stat_screenspace.vertices += vert_count;
			}

			text_vao.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);

			++stat_screenspace.entities;
		}
	}
}
