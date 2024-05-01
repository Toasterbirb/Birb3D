#pragma once

#include "EditorWindow.hpp"

namespace birb
{
	class project;
}

namespace editor
{
	class viewport;

	class project_manager : public editor_window
	{
	public:
		project_manager(birb::project& project, editor::viewport& viewport);

		void draw() override;

	private:
		birb::project& project;
		editor::viewport& viewport;

		f64 last_save = 0;
	};
}
