#include "Info.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace birb
{
	namespace component
	{
		void info::draw_editor_ui()
		{
			ImGui::InputText("Name", &name);
		}
	}
}
