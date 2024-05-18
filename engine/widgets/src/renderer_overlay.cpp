#include "Assert.hpp"
#include "Globals.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"

#include <imgui.h>
#include <vector>

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
			PROFILER_SCOPE_RENDER_FN();

			ensure(imgui_initialized);

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
				std::vector<draw_stats_table_data_point> render_stats = {
					{ "2D", renderer.rendered_2d_entities_count(), renderer.rendered_2d_vertices_count() },
					{ "3D", renderer.rendered_3d_entities_count(), renderer.rendered_3d_vertices_count() },
					{ "UI", renderer.rendered_screenspace_entities_count(), renderer.rendered_screenspace_vertices_count() },
					{ "Total", renderer.rendered_entities_count(), renderer.rendered_vertex_count() },
				};

				static const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

				ImGui::BeginTable("Drawing stats", 3, flags);
				{
					// Table headers
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Entities");
					ImGui::TableSetupColumn("Vertices");
					ImGui::TableHeadersRow();

					for (const draw_stats_table_data_point& data_point : render_stats)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("%s", data_point.name.c_str());
						ImGui::TableNextColumn();
						ImGui::Text("%u", data_point.entities);
						ImGui::TableNextColumn();
						ImGui::Text("%u", data_point.vertices);

					}
				}
				ImGui::EndTable();

				if (renderer::is_wireframe_enabled())
					ImGui::Text("> Wireframe mode enabled");
			}

			ImGui::End();
		}
	}
}
