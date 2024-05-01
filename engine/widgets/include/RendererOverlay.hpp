#pragma once

#include "WidgetBase.hpp"

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
		};
	}
}
