#pragma once

#include "EditorWindow.hpp"

namespace editor
{
	class viewport;

	class viewport_camera : public editor_window
	{
	public:
		explicit viewport_camera(viewport& viewport);
		void draw() override;

		f32 camera_scroll_strength = 0.5f;

	private:
		editor::viewport& game_viewport;
	};
}
