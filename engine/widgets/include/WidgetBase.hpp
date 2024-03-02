#pragma once

#include <imgui.h>

namespace birb
{
	struct widget_base
	{
	public:
		virtual void draw() = 0;

		float padding = 5.0f;
		float bg_alpha = 0.45f;

	protected:
		void setup_overlay();

		ImGuiWindowFlags overlay_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
	};
}
