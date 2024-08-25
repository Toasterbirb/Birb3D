#pragma once

#include "WidgetBase.hpp"

namespace birb
{
	class window;

	class world : public widget_base
	{
	public:
		explicit world(birb::window& window);

		void draw() override;

		const char* window_name = "World";

	private:
		birb::window& window;
	};
}
