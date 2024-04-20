#pragma once

#include <imgui.h>

namespace birb
{
	struct widget_base
	{
	public:
		virtual void draw() = 0;

		f32 padding = 5.0f;
		f32 bg_alpha = 0.45f;

	protected:
		const static inline ImVec4 green = ImVec4(0.49f, 0.72f, 0.34f, 1.0f);
		const static inline ImVec4 red = ImVec4(0.80f, 0.27f, 0.27f, 1.0f);

		const static inline char* overlay_window_name = "Overlays";
		void setup_overlay();
		bool is_overlay = true;
		bool p_open = false;

		ImGuiWindowFlags overlay_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
	};
}
