#include "Camera.hpp"
#include "Debugging.hpp"
#include "EntityList.hpp"
#include "Inspector.hpp"
#include "Logger.hpp"
#include "PerformanceOverlay.hpp"
#include "Profiling.hpp"
#include "Project.hpp"
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

#include <entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char** argv)
{

	birb::window window("Birb3D editor", birb::vec2<int>(1920, 1080));
	window.init_imgui();

	birb::project project;

	if (argc == 2)
		project.load(argv[1]);

	birb::scene& scene = project.scene_collection["main"];

	birb::overlay::window_info window_info(window, "Window");

	birb::timestep timestep;
	birb::overlay::performance perf_widget(timestep, "Performance");

	editor::viewport game_viewport(scene, window, timestep);
	project.load_camera_settings(game_viewport.camera);
	game_viewport.camera.editor_mode = true;

	editor::entity_list entity_list(scene);
	editor::inspector inspector(scene);
	editor::viewport_camera viewport_camera(game_viewport);
	editor::world world(window);
	editor::debugging debugging;

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
	ImGui::DockBuilderDockWindow("Debugging", right_side_node);

	while (!window.should_close())
	{
		{
			PROFILER_SCOPE_INPUT("Process window input")

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
		}

		window.clear();

		// Draw the main menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open project"))
				{
					constexpr unsigned int max_filename_len = 1024;
					char filename[max_filename_len];
					FILE* file = popen("zenity --file-selection", "r");
					fgets(filename, max_filename_len, file);
					birb::log("File name", filename);
				}

				if (ImGui::MenuItem("Save"))
				{
					birb::log("Saving the project to " + project.path());
					project.save_camera_settings(game_viewport.camera);
					project.save();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		{
			PROFILER_SCOPE_RENDER("Update docking space dimensions")

			ImVec2 win_pos = ImGui::GetMainViewport()->Pos;
			ImVec2 win_size = ImGui::GetMainViewport()->Size;

			// Make room for the main menu
			constexpr int offset = 22;
			win_pos.y += offset;
			win_size.y -= offset;

			ImGui::DockBuilderSetNodePos(dockspace_id, win_pos);
			ImGui::DockBuilderSetNodeSize(dockspace_id, win_size);
		}

		// Draw the editor windows
		{
			PROFILER_SCOPE_RENDER("Draw editor windows")
			game_viewport.draw();
			viewport_camera.draw();
			entity_list.draw();
			inspector.draw();
			world.draw();
			debugging.draw();
			perf_widget.draw();
			renderer_overlay.draw();
			window_info.draw();
		}

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
