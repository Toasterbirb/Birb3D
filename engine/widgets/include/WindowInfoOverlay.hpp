#pragma once

#include "WidgetBase.hpp"

namespace birb
{
	class window;

	namespace overlay
	{
		class window_info : public widget_base
		{
		public:
			explicit window_info(birb::window& window);
			window_info(birb::window& window, const char* collapsing_menu);

			void draw() override;

		private:
			birb::window& window;
			const char* collapsing_menu_name = "";
		};
	}
}
