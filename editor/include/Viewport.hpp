#pragma once

#include "Camera.hpp"
#include "EditorWindow.hpp"
#include "FBO.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

namespace editor
{
	class viewport : public editor_window
	{
	public:
		viewport(birb::scene& scene, birb::window& window, birb::timestep& timestep);

		void draw() override;
		bool is_window_hovered() const;
		birb::vec2<int> dimensions() const;

		birb::renderer renderer;
		birb::camera camera;

		float camera_near_clip = 0.1f;
		float camera_far_clip = 100.0f;

	private:
		birb::vec2<int> last_viewport_size = {1280, 720};
		birb::fbo fbo;

		birb::scene& scene;
		birb::window& window;
		birb::timestep& timestep;

		bool imgui_window_hovered = false;
	};
}
