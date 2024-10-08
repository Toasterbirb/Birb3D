#include "Color.hpp"
#include "EventBus.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "World.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <stb_sprintf.h>

namespace birb
{
	world::world(birb::window& window) : window(window) {}

	void world::draw()
	{
		PROFILER_SCOPE_RENDER_FN();

		// ImGui::SetNextWindowSize(ImVec2(256, 128), ImGuiCond_FirstUseEver);

		ImGui::Begin(window_name, NULL, ImGuiWindowFlags_AlwaysAutoResize);
		{
			if (ImGui::CollapsingHeader("Directional light"))
			{
				ImGui::DragFloat3("Direction", *birb::shader::directional_light.direction.to_ptr_array().data(), 0.02f);
				ImGui::Spacing();
				ImGui::ColorEdit3("Ambient", *birb::shader::directional_light.ambient.to_ptr_array().data());
				ImGui::ColorEdit3("Diffuse", *birb::shader::directional_light.diffuse.to_ptr_array().data());
				ImGui::ColorEdit3("Specular", *birb::shader::directional_light.specular.to_ptr_array().data());
				ImGui::Spacing();

				if (ImGui::Button("Apply ambient to background"))
				{
					birb::event_data data;
					data._f32 = { birb::shader::directional_light.ambient.x, birb::shader::directional_light.ambient.y, birb::shader::directional_light.ambient.z };

					birb::event_bus::send_event(birb::event::set_window_background_clear_color, data);
				}
			}

			if (ImGui::CollapsingHeader("Point lights"))
			{
				for (u16 i = 0; i < birb::shader::point_light_count; ++i)
				{
					std::string light_name = std::to_string(static_cast<unsigned int>(i)) + " " + birb::shader::point_lights[i].name;

					if (ImGui::TreeNode(light_name.c_str()))
					{
						static std::string new_name = birb::shader::point_lights[i].name;
						ImGui::InputText("Name", &new_name);
						ImGui::SameLine();
						if (ImGui::Button("Apply"))
							birb::shader::point_lights[i].name = new_name;

						ImGui::Spacing();
						ImGui::DragFloat3("Position", *birb::shader::point_lights[i].position.to_ptr_array().data());
						ImGui::Spacing();
						ImGui::SeparatorText("Color");
						ImGui::ColorEdit3("Ambient", *birb::shader::point_lights[i].ambient.to_ptr_array().data());
						ImGui::ColorEdit3("Diffuse", *birb::shader::point_lights[i].diffuse.to_ptr_array().data());
						ImGui::ColorEdit3("Specular", *birb::shader::point_lights[i].specular.to_ptr_array().data());
						ImGui::Spacing();
						ImGui::SeparatorText("Attenuation");
						ImGui::DragFloat("Constant", &birb::shader::point_lights[i].attenuation_constant, 0.05f);
						ImGui::DragFloat("Linear", &birb::shader::point_lights[i].attenuation_linear, 0.005f);
						ImGui::DragFloat("Quadratic", &birb::shader::point_lights[i].attenuation_quadratic, 0.001f);
						ImGui::TreePop();
					}
				}
			}

			if (ImGui::CollapsingHeader("Background"))
			{
				birb::color background_color = window.background_color();
				if (ImGui::ColorEdit3("Background color", *background_color.to_ptr_array().data()))
					window.set_background_color(background_color);
			}
		}
		ImGui::End();
	}
}
