#include "EventBus.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "World.hpp"

namespace editor
{
	world::world(birb::window& window) : window(window) {}

	void world::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("World");
		{
			if (ImGui::CollapsingHeader("Directional light"))
			{
				ImGui::DragFloat3("Direction", *birb::shader::directional_direction.to_ptr_array().data(), 0.02f);
				ImGui::Spacing();
				ImGui::ColorEdit3("Ambient", *birb::shader::directional_ambient.to_ptr_array().data());
				ImGui::ColorEdit3("Diffuse", *birb::shader::directional_diffuse.to_ptr_array().data());
				ImGui::ColorEdit3("Specular", *birb::shader::directional_specular.to_ptr_array().data());
				ImGui::Spacing();

				if (ImGui::Button("Apply ambient to background"))
				{
					birb::event_data data;
					data._float = { birb::shader::directional_ambient.x, birb::shader::directional_ambient.y, birb::shader::directional_ambient.z };

					birb::event_bus::send_event(2, data);
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
