#include "Camera.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "Renderer.hpp"
#include "Timestep.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Text rendering", birb::vec2<i32>(1280, 720));
	birb::timestep timestep;

	birb::scene scene;
	birb::camera camera;

	birb::renderer renderer;
	renderer.opt_blend(true);
	renderer.set_scene(scene);

	birb::font_manager font_manager;
	birb::font manaspace = font_manager.load_font("manaspc.ttf", 16);

	while (!window.should_close())
	{
		window.clear();

		// Draw here
		renderer.draw_entities(camera.get_view_matrix(), camera.get_projection_matrix(birb::camera::projection_mode::orthographic, window.size()));

		window.flip();

		window.poll();
		timestep.step();
	}
}
