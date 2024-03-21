#include "EditorComponent.hpp"

namespace birb
{
	void editor_component::draw_info_table_row(const std::string& name, const std::string& data)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%s", name.c_str());
		ImGui::TableNextColumn();
		ImGui::Text("%s", data.c_str());
	}
}
