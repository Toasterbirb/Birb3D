#include "FBO.hpp"
#include "Profiling.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Viewport.hpp"
#include "Window.hpp"

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
		PROFILER_SCOPE_RENDER_FN()

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
					last_viewport_size = birb::vec2<i32>(window_size.x, window_size.y);
					fbo.reload_frame_buffer_texture(last_viewport_size);
				}

				glViewport(0, 0, window_size.x, window_size.y);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				renderer.draw_entities(camera.view_matrix(), camera.projection_matrix(birb::camera::projection_mode::perspective, birb::vec2<i32>(window_size.x, window_size.y)));
			}
			fbo.unbind();

			ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(fbo.frame_buffer_texture().id)), window_size, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}
		ImGui::End();
	}

	birb::vec2<i32> viewport::dimensions() const
	{
		return last_viewport_size;
	}

	bool viewport::is_window_hovered() const
	{
		return imgui_window_hovered;
	}
}
