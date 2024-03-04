#pragma once

#include "EditorWindow.hpp"
#include "Window.hpp"

namespace editor
{
	class world : public editor_window
	{
	public:
		explicit world(birb::window& window);

		void draw() override;

	private:
		birb::window& window;
	};
}
