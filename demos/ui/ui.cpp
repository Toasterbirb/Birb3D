#include "Camera.hpp"
#include "Canvas.hpp"
#include "Rectangle.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Types.hpp"
#include "Vector.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("UI", birb::vec2<i32>(1280, 720));
	birb::timestep timestep;

	birb::camera camera(window.size());
	birb::renderer renderer;

	birb::scene scene;
	renderer.set_scene(scene);

	birb::ui::canvas canvas;
	renderer.set_canvas(canvas);

	birb::ui::rectangle rectangle({ 32, 32, 0 }, { 128, 64 }, 0xFF0011);
	birb::ui::rectangle rectangle2({ 128, 128, 0 }, { 256, 64 }, 0xFFFF00);

	canvas.add_ui_element(rectangle);
	canvas.add_ui_element(rectangle2);

	while (!window.should_close())
	{
		window.clear();
		renderer.draw_entities(camera, window.size());
		window.flip();

		window.poll();
		timestep.step();
	}

	return 0;
}
