#pragma once

#include "Camera.hpp"
#include "FBO.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

namespace editor
{
	class game_viewport
	{
	public:
		game_viewport(birb::scene& scene, birb::window& window, birb::timestep& timestep);

		void draw_viewport();
	private:
		birb::vec2<int> last_viewport_size = {1280, 720};
		birb::fbo fbo;

		birb::renderer renderer;
		birb::camera camera;
		birb::scene& scene;
		birb::window& window;
		birb::timestep& timestep;
	};
}
