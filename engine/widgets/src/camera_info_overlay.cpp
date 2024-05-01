#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Profiling.hpp"

#include <imgui.h>

namespace birb
{
	namespace overlay
	{
		camera_info::camera_info(const camera& camera)
		:camera_obj(camera)
		{}

		void camera_info::draw()
		{
			PROFILER_SCOPE_RENDER_FN();

			setup_overlay();

			bool p_open = false;
			ImGui::Begin("Overlay", &p_open, overlay_window_flags);
			ImGui::SeparatorText("Camera");

			ImGui::Text("Yaw: %.2f", camera_obj.yaw);
			ImGui::Text("Pitch: %.2f", camera_obj.pitch);
			ImGui::Text("Pos: [%.2f, %.2f, %.2f]", camera_obj.position.x, camera_obj.position.y, camera_obj.position.z);
			ImGui::Text("Orthographic scale: %.3f", camera_obj.orthograhpic_scale);

			ImGui::End();
		}
	}
}
