#pragma once

#include "Types.hpp"
#include "WidgetBase.hpp"

#include <array>

namespace birb
{
	class renderer;

	namespace overlay
	{
		class renderer_overlay : public widget_base
		{
		public:
			explicit renderer_overlay(const birb::renderer& renderer);
			renderer_overlay(const birb::renderer& renderer, const char* window);

			void draw() override;

		private:
			const char* collapsing_menu_name = "";

			const birb::renderer& renderer;

			std::array<f64, 256> total_render_duration_history = { 0.0 };

			struct draw_stats_table_data_point
			{
				std::string name;
				u32 entities;
				u32 vertices;
				f64 time;
			};
		};
	}
}
