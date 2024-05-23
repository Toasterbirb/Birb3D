#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "PerformanceOverlay.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "Stopwatch.hpp"
#include "Text.hpp"
#include "Timestep.hpp"
#include "Types.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <string>

static const std::string gnu_linux = "I'd just like to interject for a moment. What you're refering to as Linux, is in fact,\n\
GNU/Linux, or as I've recently taken to calling it, GNU plus Linux. Linux is not an\n\
operating system unto itself, but rather another free component of a fully functioning\n\
GNU system made useful by the GNU corelibs, shell utilities and vital system components\n\
comprising a full OS as defined by POSIX.\n\
Many computer users run a modified version of the GNU system every day, without realizing\n\
it. Through a peculiar turn of events, the version of GNU which is widely used today is\n\
often called Linux, and many of its users are not aware that it is basically the GNU\n\
system, developed by the GNU Project.\n\
There really is a Linux, and these people are using it, but it is just a part of the\n\
system they use. Linux is the kernel: the program in the system that allocates the\n\
machine's resources to the other programs that you run. The kernel is an essential part\n\
of an operating system, but useless by itself; it can only function in the context of a\n\
complete operating system. Linux is normally used in combination with the GNU operating\n\
system: the whole system is basically GNU with Linux added, or GNU/Linux. All the so-\n\
called Linux distributions are really distributions of GNU/Linux!";

static const std::string manaspace_ttf_font_name = "manaspc.ttf";
static constexpr f32 text_scroll_speed = 10.0f;

int main(void)
{
	birb::window window("Benchmark", birb::vec2<i32>(1920, 822), false);
	window.init_imgui();

	birb::scene scene;
	birb::renderer renderer;
	renderer.set_scene(scene);

	birb::camera camera;

	birb::timestep timestep;
	timestep.disable_fps_cap = true;

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	///////////////////////////////
	// Create variables and such //
	///////////////////////////////

	birb::stopwatch stopwatch_init_font("Font loading");
	birb::font_manager font_manager;
	birb::font manaspace_small = font_manager.load_font(manaspace_ttf_font_name, 12);
	birb::font manaspace_big = font_manager.load_font(manaspace_ttf_font_name, 32);
	stopwatch_init_font.stop();

	birb::stopwatch stopwatch_init_text("Text entity creation");
	birb::text text_small_t(gnu_linux, manaspace_small, birb::vec3<f32>(32.0f, window.size().y, 0.0f), 1.0f, 0xb48ead);
	birb::text text_big_t(gnu_linux, manaspace_big, birb::vec3<f32>(32.0f, window.size().y, 0.0f), 0.99f, 0xa3be8c);

	birb::entity text_small = scene.create_entity();
	text_small.add_component(text_small_t);

	birb::entity text_big = scene.create_entity();
	text_big.add_component(text_big_t);

	stopwatch_init_text.stop();


	while (!window.should_close())
	{
		// Move the text entities down with a constant speed
		text_small.get_component<birb::text>().position.y += timestep.deltatime() * text_scroll_speed;
		text_big.get_component<birb::text>().position.y += timestep.deltatime() * text_scroll_speed;

		window.clear();

		renderer.draw_entities(camera, window.size());
		performance_overlay.draw();
		renderer_overlay.draw();

		window.flip();

		window.poll();
		timestep.step();
	}
}
