#include "Profiling.hpp"
#include "Viewport.hpp"
#include "Viewport.hpp"
#include "ViewportCamera.hpp"

namespace editor
{
	viewport_camera::viewport_camera(editor::viewport& viewport) : game_viewport(viewport) {}

	void viewport_camera::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Viewport camera");
		{
			ImGui::Text("Viewport size: [%d, %d]", game_viewport.dimensions().x, game_viewport.dimensions().y);
			ImGui::Spacing();

			// Position slider
			{
				std::array<f32, 3> camera_position_arr = { game_viewport.camera.position.x, game_viewport.camera.position.y, game_viewport.camera.position.z };
				ImGui::DragFloat3("Position", camera_position_arr.data(), 0.2f);

				// Apply the position array
				game_viewport.camera.position = { camera_position_arr[0], camera_position_arr[1], camera_position_arr[2] };
			}

			// Rotation sliders
			{
				std::array<f32, 2> camera_rotation_arr = { game_viewport.camera.pitch, game_viewport.camera.yaw };
				ImGui::DragFloat2("Rotation", camera_rotation_arr.data(), 1.0f, 0.0f, 0.0f, "%.3f");

				// Apply the rotation array
				game_viewport.camera.pitch = camera_rotation_arr[0];
				game_viewport.camera.yaw = camera_rotation_arr[1];
				game_viewport.camera.update_camera_vectors();
			}

            ImGui::SliderFloat("FOV", &game_viewport.camera.fov, 1.0f, 120.0f, "%.1f");
			ImGui::DragFloatRange2("Clipping", &game_viewport.camera.near_clip, &game_viewport.camera.far_clip, 0.1f, 0.001f, 1000.0f, "Near: %.3f", "Far: %.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::InputFloat("Scroll speed", &camera_scroll_strength, 0.25f);
			ImGui::InputFloat("Movement speed", &game_viewport.camera.movement_speed, 0.25f);

			bool wireframe_mode_enabled = birb::renderer::is_wireframe_enabled();
			if (ImGui::Checkbox("Wireframe", &wireframe_mode_enabled))
				birb::renderer::toggle_wireframe();

			bool backface_culling_enabled = birb::renderer::is_backface_culling_enabled();
			if (ImGui::Checkbox("Backface culling", &backface_culling_enabled))
				birb::renderer::set_backface_culling(backface_culling_enabled);
		}
		ImGui::End();
	}
}
