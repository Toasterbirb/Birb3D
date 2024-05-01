#pragma once

#include "EditorWindow.hpp"

namespace birb
{
	class project;
}

namespace editor
{
	class viewport;

	class main_menu_bar : public editor_window
	{
	public:
		explicit main_menu_bar(birb::project& project, editor::viewport& viewport);
		~main_menu_bar() = default;
		main_menu_bar(const main_menu_bar&) = default;
		main_menu_bar(main_menu_bar&) = default;

		void save_project();
		void draw() override;

	private:
		birb::project& project;
		editor::viewport& viewport;
	};
}
