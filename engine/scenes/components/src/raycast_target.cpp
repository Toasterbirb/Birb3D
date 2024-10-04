#include "RaycastTarget.hpp"

#include <imgui.h>

namespace birb
{
	void raycast_target::draw_editor_ui()
	{
		ImGui::InputFloat3("Position", *position.to_ptr_array().data());
		ImGui::InputFloat("Radius", &radius);
	}

	std::string raycast_target::collapsing_header_name() const
	{
		return editor_header_name;
	}
}
