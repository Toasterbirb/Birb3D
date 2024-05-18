#include "Material.hpp"

namespace birb
{
	material::material()
	:diffuse(0x112233), specular(0x112233), shininess(32)
	{}

	material::material(const color& diffuse, const color& specular, f32 shininess)
	:diffuse(diffuse), specular(specular), shininess(shininess)
	{}

	void material::draw_editor_ui()
	{
		ImGui::ColorEdit3("Diffuse", *diffuse.to_ptr_array().data());
		ImGui::ColorEdit3("Specular", *specular.to_ptr_array().data());
		ImGui::DragFloat("shininess", &shininess, 1.0f, 0.1f, 2048.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
	}

	std::string material::collapsing_header_name() const
	{
		return editor_header_name;
	}
}
