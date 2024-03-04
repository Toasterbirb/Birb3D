#include "Viewport.hpp"
#include "FBO.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace editor
{
	viewport::viewport(birb::scene& scene, birb::window& window, birb::timestep& timestep)
	:fbo(last_viewport_size), scene(scene), window(window), timestep(timestep)
	{
		renderer.set_scene(scene);
	}

	void viewport::draw()
	{
		// Render the game viewport
		ImGui::Begin("Viewport");
		{
			ImGui::BeginChild("ViewportRenderview");
			ImVec2 window_size = ImGui::GetWindowSize();
			imgui_window_hovered = ImGui::IsWindowHovered();

			if (imgui_window_hovered)
				camera.process_input(window, timestep);


			// Render the game view
			fbo.bind();
			{
				if (window_size.x != last_viewport_size.x || window_size.y != last_viewport_size.y)
				{
					last_viewport_size = birb::vec2<int>(window_size.x, window_size.y);
					fbo.reload_frame_buffer_texture(last_viewport_size);
				}

				glViewport(0, 0, window_size.x, window_size.y);
				glm::mat4 projection = glm::perspective(glm::radians(camera.fov), window_size.x / window_size.y, camera_near_clip, camera_far_clip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				renderer.draw_entities(camera.get_view_matrix(), projection);
			}
			fbo.unbind();

			ImGui::Image((void *)(intptr_t)fbo.frame_buffer_texture().id, window_size, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}
		ImGui::End();
	}

	bool viewport::is_window_hovered() const
	{
		return imgui_window_hovered;
	}
}
