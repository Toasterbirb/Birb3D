#include "Camera.hpp"
#include "EntityList.hpp"
#include "Inspector.hpp"
#include "Logger.hpp"
#include "PerformanceOverlay.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Viewport.hpp"
#include "ViewportCamera.hpp"
#include "Window.hpp"
#include "WindowInfoOverlay.hpp"
#include "World.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

#include <array>
#include <entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
	birb::window window("Birb3D editor", birb::vec2<int>(1920, 1080));
	window.init_imgui();

	birb::overlay::window_info window_info(window, "Window");

	birb::timestep timestep;
	birb::overlay::performance perf_widget(timestep, "Performance");

	birb::scene scene;

	editor::viewport game_viewport(scene, window, timestep);
	game_viewport.camera.editor_mode = true;

	editor::entity_list entity_list(scene);
	editor::inspector inspector(scene);
	editor::viewport_camera viewport_camera(game_viewport);
	editor::world world(window);

	birb::overlay::renderer_overlay renderer_overlay(game_viewport.renderer, "Viewport renderer");

	// Setup docking
	ImGuiID dockspace_id = ImGui::GetID("Viewport");
	ImGui::DockBuilderAddNode(dockspace_id);
	ImGuiID viewport_node;
	ImGuiID viewport_camera_node;
	ImGuiID right_side_node;
	ImGuiID left_side_node;
	ImGuiID left_side_bottom_node;

	ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.7f, &viewport_node, &right_side_node);
	ImGui::DockBuilderSplitNode(right_side_node, ImGuiDir_Down, 0.8f, &right_side_node, &viewport_camera_node);
	ImGui::DockBuilderSplitNode(viewport_node, ImGuiDir_Left, 0.2f, &left_side_node, &viewport_node);
	ImGui::DockBuilderSplitNode(left_side_node, ImGuiDir_Up, 0.5f, &left_side_node, &left_side_bottom_node);

	ImGui::DockBuilderDockWindow("Viewport", viewport_node);
	ImGui::DockBuilderDockWindow("Overlays", right_side_node);
	ImGui::DockBuilderDockWindow("Viewport camera", viewport_camera_node);
	ImGui::DockBuilderDockWindow("Entities", left_side_node);
	ImGui::DockBuilderDockWindow("Inspector", left_side_bottom_node);
	ImGui::DockBuilderDockWindow("World", right_side_node);

	while (!window.should_close())
	{
		while (window.inputs_available())
		{
			birb::input input = window.next_input();

			if (input.state == birb::input::action::KEY_DOWN)
			{
				switch (input.key)
				{
					case birb::input::keycode::SCROLLING:
					{
						// Scroll the viewport view only if we are hoving over it
						if (game_viewport.is_window_hovered())
						{
							if (window.is_key_held(birb::input::keycode::LEFT_SHIFT))
								game_viewport.camera.position.y += input.offset.y * viewport_camera.camera_scroll_strength;
							else if (window.is_key_held(birb::input::keycode::LEFT_CONTROL))
								game_viewport.camera.position += (static_cast<float>(input.offset.y * viewport_camera.camera_scroll_strength) * game_viewport.camera.right_vec());
							else
								game_viewport.camera.zoom(input.offset.y * viewport_camera.camera_scroll_strength);
						}
						break;
					}

					default:
						break;
				}
			}
		}

		window.clear();


		// Update the dimensions of the docking space
		ImGui::DockBuilderSetNodePos(dockspace_id, ImGui::GetMainViewport()->Pos);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

		// Draw the editor windows
		game_viewport.draw();
		viewport_camera.draw();
		entity_list.draw();
		inspector.draw();
		world.draw();

		perf_widget.draw();
		renderer_overlay.draw();
		window_info.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
