#include "Profiling.hpp"
#include "RendererOverlay.hpp"
#include "Window.hpp"
#include "imgui.h"

namespace birb
{
	namespace overlay
	{
		renderer_overlay::renderer_overlay(const birb::renderer& renderer)
		:renderer(renderer)
		{}

		void renderer_overlay::draw()
		{
			PROFILER_SCOPE_RENDER_FN()

			setup_overlay();

			bool p_open = false;
			ImGui::Begin("Overlay", &p_open, overlay_window_flags);
			ImGui::SeparatorText("Renderer");

			ImGui::Text("Rendered entities: %u", renderer.rendered_entities_count());

			if (renderer::is_wireframe_enabled())
				ImGui::Text("> Wireframe mode enabled");

			ImGui::End();
		}
	}
}
