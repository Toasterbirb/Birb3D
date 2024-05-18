#include "Info.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace birb
{
	void info::draw_editor_ui()
	{
		ImGui::InputText("Name", &name);
	}

	std::string info::collapsing_header_name() const
	{
		return editor_header_name;
	}
}
