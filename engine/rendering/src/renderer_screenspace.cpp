#include "Line.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "State.hpp"
#include "Text.hpp"

#include <glad/gl.h>

namespace birb
{
	void renderer::draw_screenspace_entities()
	{
		PROFILER_SCOPE_RENDER_FN();

		draw_canvas();
		draw_lines();
		draw_text();
	}

	void renderer::draw_canvas()
	{
		// Skip drawing the canvas if one has not been set
		// There might be a game that has no "UI"

		if (current_canvas != nullptr)
		{
			// Drawing 2D things usually doesn't work too well with backface culling
			bool backface_culling_enabled = is_backface_culling_enabled();
			set_backface_culling(false);

			current_canvas->draw();

			if (backface_culling_enabled)
				set_backface_culling(true);
		}
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

		// We'll be drawing to TEXTURE0
		glActiveTexture(GL_TEXTURE0);

		for (const auto& entity : view)
		{
			// Check if the entity should be skipped because its not active
			if (!current_scene->is_entity_active(entity))
				continue;

			const birb::text& text = view.get<birb::text>(entity);

			// Skip the entity if the text component doens't have any text to render
			if (text.empty())
				continue;

			// Fetch the shader
			const std::shared_ptr<shader> shader = shader_collection::get_shader(text.shader);
			ensure(shader->id != 0, "Tried to use an invalid shader for rendering");

			shader->activate();
			shader->set(shader_uniforms::text_color, text.color);
			shader->set(shader_uniforms::text_position, text.position);

			// The same VAO can be used for all characters
			text_vao.bind();

			const std::set<char>& chars = text.chars();

			// Set the text vertex attrib pointer
			text_vbo.bind();
			text_vbo.set_vertex_attrib_ptr(0, 0, sizeof(f32), 4);

			// Iterate through the text
			for (const char c : chars)
			{
				// Update the VBO
				constexpr u8 vert_count = 6;

				const vec2<f32>& dim = text.font.char_dimensions(c).to_float();

				const f32 verts[vert_count][4] = {
					{ 0,		dim.y,	0.0f, 0.0f },
					{ 0,		0,		0.0f, 1.0f },
					{ dim.x, 	0,		1.0f, 1.0f },

					{ 0,		dim.y,	0.0f, 0.0f },
					{ dim.x,	0, 		1.0f, 1.0f },
					{ dim.x,	dim.y,	1.0f, 0.0f }
				};

				texture::bind(text.char_texture_id(c));

				text_vbo.bind();
				text_vbo.update_data(*verts, sizeof(verts));

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
			texture::unbind();

			++render_stats.entities_screenspace;
		}

	}
}
