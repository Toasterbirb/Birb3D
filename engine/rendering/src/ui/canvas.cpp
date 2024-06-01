#include "Canvas.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <glad/gl.h>
#include <memory>

namespace birb
{
	namespace ui
	{
		canvas::canvas()
		:ui_shader("ui")
		{}

		void canvas::add_ui_element(ui_base& ui_element)
		{
#ifndef NDEBUG
			// Make sure that the same UI-element doesn't get added multiple times
			// This needs to be prevented because they would just get drawn over eachother
			{
				bool match_found = false;
				for (size_t i = 0; i < ui_elements.size(); ++i)
				{
					if (ui_elements.at(i) == &ui_element)
					{
						match_found = true;
						break;
					}
				}

				ensure(match_found == false, "Tried to add a duplicate UI-element into the canvas");
			}
#endif

			ui_elements.push_back(&ui_element);
		}

		void canvas::draw()
		{
			// Activate the UI-shader
			std::shared_ptr<shader> shader = shader_collection::get_shader(ui_shader);
			shader->activate();

			for (u32 i = 0; i < ui_elements.size(); ++i)
			{
				const std::vector<f32> vertices = ui_elements.at(i)->vertex_data();
				ensure(vertices.size() > 2);

				shader->set(shader_uniforms::color_alpha, ui_elements.at(i)->color());

				// TODO: The VBO and VAO should be pre-created somewhere and recycled
				vao vao;
				vbo vbo;

				vao.bind();
				vbo.bind();

				vbo.set_data(vertices, false);
				vao.link_vbo(vbo, 0, 3, 3, 0);

				vao.unbind();
				vbo.unbind();

				vao.bind();

				glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			}
		}
	}
}
