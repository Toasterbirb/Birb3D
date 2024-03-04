#pragma once

#include "EditorWindow.hpp"
#include "Viewport.hpp"

namespace editor
{
	class viewport_camera : public editor_window
	{
	public:
		explicit viewport_camera(viewport& viewport);
		void draw() override;

		float camera_scroll_strength = 0.5f;

	private:
		editor::viewport& game_viewport;
	};
}
