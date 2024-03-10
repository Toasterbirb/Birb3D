#pragma once

#include "EditorWindow.hpp"
#include "Project.hpp"
#include "Viewport.hpp"

namespace editor
{
	class project_manager : public editor_window
	{
	public:
		project_manager(birb::project& project, editor::viewport& viewport);

		void draw() override;

	private:
		birb::project& project;
		editor::viewport& viewport;

		double last_save = 0;
	};
}
