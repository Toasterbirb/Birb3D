#include "Transform.hpp"

#include <imgui.h>

namespace birb
{
	namespace component
	{
		transform::transform()
		:position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), local_scale(1.0f, 1.0f, 1.0f)
		{}

		void transform::draw_editor_ui()
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::DragFloat3("Position", *position.to_ptr_array().data(), 0.25f);
				ImGui::DragFloat3("Rotation", *rotation.to_ptr_array().data(), 0.25f);
				ImGui::Separator();
				ImGui::DragFloat3("Local scale", *local_scale.to_ptr_array().data(), 0.1f);
				if (ImGui::DragFloat("Combined", &local_scale.x, 0.1f))
				{
					local_scale.y = local_scale.x;
					local_scale.z = local_scale.x;
				}
			}
		}
	}
}
