#include "Assert.hpp"
#include "Globals.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Stopwatch.hpp"

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
				const renderer::statistics stats = renderer.rendering_statistics();
				f64 total_duration = stats.total_duration();

				std::vector<draw_stats_table_data_point> render_stats = {
					{ "2D", stats.entities_2d, stats.vertices_2d, stats.draw_2d_duration / total_duration },
					{ "3D", stats.entities_3d, stats.vertices_3d, stats.draw_3d_duration / total_duration },
					{ "UI", stats.entities_screenspace, stats.vertices_screenspace, stats.draw_screenspace_duration / total_duration },
					{ "Total", stats.total_entities(), stats.total_vertices(), -1.0 },
				};

				static const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

				ImGui::BeginTable("Drawing stats", 4, flags);
				{
					// Table headers
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Entities");
					ImGui::TableSetupColumn("Vertices");
					ImGui::TableSetupColumn("Time");
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
						ImGui::TableNextColumn();

						// Only the "total" time will be -1 and that one should be drawn differently
						if (data_point.time != -1)
							ImGui::ProgressBar(data_point.time, ImVec2(120, 0));
						else
							ImGui::Text("%s", stopwatch::format_time(total_duration).c_str());
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
