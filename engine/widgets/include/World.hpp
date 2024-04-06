#pragma once

#include "WidgetBase.hpp"
#include "Window.hpp"

namespace birb
{
	class world : public widget_base
	{
	public:
		explicit world(birb::window& window);

		void draw() override;

	private:
		birb::window& window;
	};
}
