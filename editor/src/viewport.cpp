#include "Viewport.hpp"
#include "FBO.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace editor
{
	game_viewport::game_viewport(birb::scene& scene, birb::window& window, birb::timestep& timestep)
	:fbo(last_viewport_size), scene(scene), window(window), timestep(timestep)
	{
		renderer.set_scene(scene);
	}

	void game_viewport::draw_viewport()
	{
		camera.process_input(window, timestep);

		// Render the game viewport
		ImGui::Begin("GameWindow");
		{
			ImGui::BeginChild("Viewport");
			ImVec2 window_size = ImGui::GetWindowSize();

			// Render the game view
			fbo.bind();
			{
				if (window_size.x != last_viewport_size.x || window_size.y != last_viewport_size.y)
				{
					last_viewport_size = birb::vec2<int>(window_size.x, window_size.y);
					fbo.reload_frame_buffer_texture(last_viewport_size);
				}

				glm::mat4 projection = glm::perspective(glm::radians(45.0f), window_size.x / window_size.y, 0.1f, 100.0f);

				glViewport(0, 0, window_size.x, window_size.y);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderer.draw_entities(camera.get_view_matrix(), projection);
			}
			fbo.unbind();

			ImGui::Image((void *)(intptr_t)fbo.frame_buffer_texture().id, window_size, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}
		ImGui::End();
	}
}
