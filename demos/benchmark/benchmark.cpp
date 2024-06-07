#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "ShaderCollection.hpp"
#include "Sprite.hpp"
#include "Stopwatch.hpp"
#include "Text.hpp"
#include "Timer.hpp"
#include "Timestep.hpp"
#include "Types.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <algorithm>
#include <string>
#include <sys/resource.h>

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
static constexpr i32 text_entity_count = 6;

static constexpr i32 normal_sprite_count = 64;
static constexpr i32 transformer_sprite_count = 128;
static constexpr f32 sprite_scroll_speed = 200.0f;
static constexpr f32 sprite_rotation_speed = 10.0f;

static constexpr i32 suzanne_count = 14;
static constexpr i32 suzanne_row_count = 4;

int main(void)
{
	birb::stopwatch stopwatch_benchmark("Benchmark");

	birb::stopwatch stopwatch_window_creation("Window creation");
	birb::window window("Benchmark", birb::vec2<i32>(1920, 822));
	f64 window_creation_time = stopwatch_window_creation.stop();

	window.init_imgui();

	birb::stopwatch stopwatch_shader_precompiling;
	birb::shader_collection::precompile_basic_shaders();
	f64 shader_precompiling_time = stopwatch_shader_precompiling.stop();

	birb::stopwatch stopwatch_renderer_construction("Renderer construction");
	birb::renderer renderer;
	f64 renderer_construction_time = stopwatch_renderer_construction.stop();

	birb::scene scene;
	renderer.set_scene(scene);
	renderer.opt_gamma_correction(false);

	birb::camera camera(window.size());
	camera.position.z = 50;

	birb::timestep timestep;
	timestep.disable_fps_cap = true;

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	///////////////////////////////
	// Create variables and such //
	///////////////////////////////

	// Benchmarking variables //
	constexpr i32 benchmark_array_reserve_size = 2000;
	u64 draw_call_count = 0;

	std::vector<f64> deltatimes;
	deltatimes.reserve(benchmark_array_reserve_size);

	std::vector<f64> render_2d_times;
	render_2d_times.reserve(benchmark_array_reserve_size);

	std::vector<f64> render_3d_times;
	render_3d_times.reserve(benchmark_array_reserve_size);

	std::vector<f64> render_screenspace_times;
	render_screenspace_times.reserve(benchmark_array_reserve_size);

	std::vector<f64> draw_entities_times;
	draw_entities_times.reserve(benchmark_array_reserve_size);

	// Text //

	birb::stopwatch stopwatch_init_font("Font loading");
	birb::font_manager font_manager;
	birb::font manaspace_small = font_manager.load_font(manaspace_ttf_font_name, 12);
	birb::font manaspace_big = font_manager.load_font(manaspace_ttf_font_name, 24);
	stopwatch_init_font.stop();


	birb::stopwatch stopwatch_init_text("Text entity creation");
	birb::text text_small_t(gnu_linux, manaspace_small, birb::vec3<f32>(512.0f, 1024.0f, 0.0f), 0xb48ead);
	birb::text text_big_t(gnu_linux, manaspace_big, birb::vec3<f32>(32.0f, 1024.0f, 0.0f), 0xa3be8c);

	std::vector<birb::entity> moving_text_entities;
	std::vector<birb::entity> changing_text_entities;

	for (i32 i = 0; i < text_entity_count; ++i)
	{
		birb::entity text_small = scene.create_entity();
		text_small.add_component(text_small_t);
		moving_text_entities.push_back(text_small);

		birb::entity text_big = scene.create_entity();
		text_big.add_component(text_big_t);
		moving_text_entities.push_back(text_big);

		birb::entity text_stationary_not_changing = scene.create_entity();
		text_stationary_not_changing.add_component(text_big_t);
		text_stationary_not_changing.get_component<birb::text>().set_text("Stationary text");
		text_stationary_not_changing.get_component<birb::text>().position = { 1500.0f, 1024 - (32.0f * (i + 1)), 0.0f };

		birb::entity text_stationary_changing = scene.create_entity();
		text_stationary_changing.add_component(text_big_t);
		text_stationary_changing.get_component<birb::text>().set_text("Stationary changing text");
		text_stationary_changing.get_component<birb::text>().position = { 1900.0f, 1024 - (32.0f * (i + 1)), 0.0f };
		changing_text_entities.push_back(text_stationary_changing);
	}

	f64 text_init_time = stopwatch_init_text.stop();

	// Sprites //

	birb::stopwatch stopwatch_init_sprite("Sprite construction");

	birb::sprite sprite_s("texture_512.png", birb::color_format::RGB);
	constexpr f32 sprite_position_offset = 3.0f;

	for (i32 i = 0; i < normal_sprite_count; ++i)
	{
		birb::entity sprite = scene.create_entity(birb::component::transform);
		sprite.add_component(sprite_s);

		sprite.get_component<birb::transform>().position = { -10.0f * sprite_position_offset * i, 150.0f, static_cast<f32>(i) };
		sprite.get_component<birb::transform>().local_scale = { 200.0f, 200.0f + (i * 1.2f), 1.0f };
	}

	birb::entity transformer_sprite = scene.create_entity(birb::component::transformer);
	transformer_sprite.add_component(sprite_s);
	constexpr f32 sprite_x_pos_start = 256.0f;
	f32 sprite_x_pos = sprite_x_pos_start;
	f32 sprite_height = 64.0f;
	for (i32 i = 0; i < transformer_sprite_count; ++i)
	{
		birb::transform t;
		t.position = { sprite_x_pos + (i % 8) * 10.0f, sprite_height, i * 0.01f };
		t.local_scale = { i * 2.0f, i * 2.0f, 1.0f };
		t.rotation.z = i * 5.0f;
		transformer_sprite.get_component<birb::transformer>().transforms.push_back(t);

		// 8 sprites per row
		if (i % 8 == 0)
		{
			sprite_height += 32.0f;
			sprite_x_pos = sprite_x_pos_start;
		}
	}
	transformer_sprite.get_component<birb::transformer>().lock();

	f64 sprite_init_time = stopwatch_init_sprite.stop();


	// 3D things //

	birb::stopwatch stopwatch_init_3d("3D model construction");

	birb::model suzanne_m("suzanne.obj");

	// birb::entity center_suzanne = scene.create_entity(birb::component::transform | birb::component::default_shader);
	// center_suzanne.add_component(suzanne_m);
	// center_suzanne.get_component<birb::transform>().position = { 0.0f, 0.0f, 0.0f };

	constexpr f32 suzanne_side_movemewnt = 1.6f;
	constexpr f32 suzanne_rotation = 5.0f;
	constexpr f32 suzanne_row_height_offset = 7.0f;

	for (i32 j = 0; j < suzanne_row_count; ++j)
	{
		for (i32 i = 0; i < suzanne_count; ++i)
		{
			birb::entity suzanne_left = scene.create_entity(birb::component::transform | birb::component::default_shader);
			suzanne_left.add_component(suzanne_m);
			suzanne_left.get_component<birb::transform>().position = { -i * suzanne_side_movemewnt, j * 2.5f - suzanne_row_height_offset, i * 2.0f };
			suzanne_left.get_component<birb::transform>().rotation.y = i * suzanne_rotation;

			birb::entity suzanne_right = scene.create_entity(birb::component::transform | birb::component::default_shader);
			suzanne_right.add_component(suzanne_m);
			suzanne_right.get_component<birb::transform>().position = { i * suzanne_side_movemewnt, j * 2.5f - suzanne_row_height_offset, i * 2.0f };
			suzanne_right.get_component<birb::transform>().rotation.y = -i * suzanne_rotation;

			// Lock the transforms in suzanne entities on the left side
			suzanne_left.get_component<birb::transform>().lock();
		}
	}

	f64 three_d_init_time = stopwatch_init_3d.stop();

	birb::timer benchmark_timer(20.0f);
	u64 frame_counter = 0;

	while (!window.should_close())
	{
		// Move the text entities down with a constant speeds
		{
			f32 text_fall_speed_multiplier = 1.0f;
			for (birb::entity text : moving_text_entities)
			{
				text.get_component<birb::text>().position.y -= timestep.deltatime() * text_scroll_speed * text_fall_speed_multiplier;
				text_fall_speed_multiplier += 0.1f;
			}
		}

		// Update the changing text entities
		{
			u32 counter = frame_counter;
			for (birb::entity text : changing_text_entities)
			{
				text.get_component<birb::text>().set_text("Stationary changing text " + std::to_string(counter % 128));
				counter += 7;
			}
		}

		// Move all of the sprites to the right and rotate them
		{
			const auto sprite_view = scene.registry.view<birb::sprite, birb::transform>();
			for (auto sprite : sprite_view)
			{
				sprite_view.get<birb::transform>(sprite).position.x += timestep.deltatime() * sprite_scroll_speed;
				sprite_view.get<birb::transform>(sprite).rotation.z += timestep.deltatime() * sprite_rotation_speed;
			}
		}

		// Rotate every 8th transformer sprite
		{
			const auto view = scene.registry.view<birb::transformer>();
			for (auto entity : view)
			{
				birb::transformer& transformer = view.get<birb::transformer>(entity);
				for (size_t i = 0; i < transformer.transforms.size() - 8; i += 8)
				{
					transformer.transforms[i].rotation.z += timestep.deltatime() * 32.0f;
					transformer.update_transform(i);
				}
				transformer.update_vbo_data();
			}
		}

		// Rotate 3D models
		{
			birb::random rng(42);

			const auto view = scene.registry.view<birb::model, birb::transform>();
			for (auto entity : view)
			{
				view.get<birb::transform>(entity).rotation.x += timestep.deltatime() * rng.range_float(10.0f, 80.0f);
				view.get<birb::transform>(entity).rotation.z += timestep.deltatime() * rng.range_float(10.0f, 80.0f);
			}
		}


		window.clear();

		birb::stopwatch stopwatch_draw_entities("Draw entities");
		renderer.draw_entities(camera, window.size());
		draw_entities_times.push_back(stopwatch_draw_entities.stop(true));

		performance_overlay.draw();
		renderer_overlay.draw();

		window.flip();

		window.poll();
		timestep.step();

		// Handle benchmark stats
		++frame_counter;
		deltatimes.push_back(timestep.deltatime());
		render_2d_times.push_back(renderer.rendering_statistics().draw_2d_duration);
		render_3d_times.push_back(renderer.rendering_statistics().draw_3d_duration);
		render_screenspace_times.push_back(renderer.rendering_statistics().draw_screenspace_duration);
		draw_call_count += renderer.rendering_statistics().total_draw_calls();
		benchmark_timer.tick(timestep.deltatime());

		if (benchmark_timer.done())
		{
			f64 benchmark_time = stopwatch_benchmark.stop();

			// Ignore the first frametime result
			// This is to prevent the laggy first time from skewing the results
			std::rotate(deltatimes.begin(), deltatimes.begin() + 1, deltatimes.end());
			deltatimes.pop_back();


			f64 average_frametime = birb::average(deltatimes);
			f64 min_frametime = birb::min_element(deltatimes);
			f64 max_frametime = birb::max_element(deltatimes);

			f64 average_fps = 1.0 / average_frametime;
			f64 min_fps = 1.0f / max_frametime;
			f64 max_fps = 1.0f / min_frametime;

			f64 average_2d_render = birb::average(render_2d_times);
			f64 average_3d_render = birb::average(render_3d_times);
			f64 average_screenspace_render = birb::average(render_screenspace_times);
			f64 average_draw_entities = birb::average(draw_entities_times);

			i32 PID = RUSAGE_SELF;
			struct rusage mem;
			getrusage(PID, &mem);

			std::cerr << "######################################\n";
			std::cerr << "Frame count: " << frame_counter << "\n";
			std::cerr << "\n";
			std::cerr << "Memory usage: " << mem.ru_maxrss / 1024 << "mb\n";
			std::cerr << "\n";
			std::cerr << "Average frametime: " << birb::stopwatch::format_time(average_frametime) << "\n";
			std::cerr << "Min frametime:     " << birb::stopwatch::format_time(min_frametime) << "\n";
			std::cerr << "Max frametime:     " << birb::stopwatch::format_time(max_frametime) << "\n";
			std::cerr << "\n";
			std::cerr << "Average FPS: " << average_fps << "\n";
			std::cerr << "Min FPS:     " << min_fps << "\n";
			std::cerr << "Max FPS:     " << max_fps << "\n";
			std::cerr << "\n";
			std::cerr << "Average render 2D:          " << birb::stopwatch::format_time(average_2d_render) << "\n";
			std::cerr << "Average render 3D:          " << birb::stopwatch::format_time(average_3d_render) << "\n";
			std::cerr << "Average render screenspace: " << birb::stopwatch::format_time(average_screenspace_render) << "\n";
			std::cerr << "Average draw_entities():    " << birb::stopwatch::format_time(average_draw_entities) << "\n";
			std::cerr << "\n";
			std::cerr << "Total draw calls:           " << draw_call_count << "\n";
			std::cerr << "Average draw calls / frame: " << draw_call_count / static_cast<double>(frame_counter) << "\n";
			std::cerr << "\n";
			std::cerr << "Window creation:       " << birb::stopwatch::format_time(window_creation_time) << "\n";
			std::cerr << "Renderer construction: " << birb::stopwatch::format_time(renderer_construction_time) << "\n";
			std::cerr << "Shader precompiling:   " << birb::stopwatch::format_time(shader_precompiling_time) << "\n";
			std::cerr << "Text init:             " << birb::stopwatch::format_time(text_init_time) << "\n";
			std::cerr << "Sprite init:           " << birb::stopwatch::format_time(sprite_init_time) << "\n";
			std::cerr << "3D init:               " << birb::stopwatch::format_time(three_d_init_time) << "\n";
			std::cerr << "\n";
			std::cerr << "Total benchmark duration: " << birb::stopwatch::format_time(benchmark_time) << "\n";

			std::cerr << "\n";

#ifndef NDEBUG
			std::cerr << "Release build: no\n";
#else
			std::cerr << "Release build: yes\n";
#endif

#if MICROPROFILE_ENABLED == 1
			std::cerr << "Profiler enabled: yes\n";
#else
			std::cerr << "Profiler enabled: no\n";
#endif

			std::cerr << "######################################\n";
			window.quit();
		}
	}
}
