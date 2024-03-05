#include "Profiling.hpp"
#include "RendererOverlay.hpp"
#include "imgui.h"

namespace birb
{
	namespace overlay
	{
		renderer_overlay::renderer_overlay(const birb::renderer& renderer)
		:renderer(renderer)
		{}

		renderer_overlay::renderer_overlay(const birb::renderer& renderer, const char* collapsing_menu)
		:collapsing_menu_name(collapsing_menu), renderer(renderer)
		{
			is_overlay = false;
		}

		void renderer_overlay::draw()
		{
			PROFILER_SCOPE_RENDER_FN()


			bool p_open = false;

			if (is_overlay)
			{
				setup_overlay();
				ImGui::Begin("Overlay", &p_open, overlay_window_flags);
				ImGui::SeparatorText("Renderer");
			}
			else
			{
				ImGui::Begin(overlay_window_name, &p_open);
			}

			if (is_overlay || ImGui::CollapsingHeader(collapsing_menu_name))
			{
				ImGui::Text("Rendered entities: %u", renderer.rendered_entities_count());
				ImGui::Text("Vertex count: %u", renderer.rendered_vertex_count());

				if (renderer::is_wireframe_enabled())
					ImGui::Text("> Wireframe mode enabled");
			}

			ImGui::End();
		}
	}
}
