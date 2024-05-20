#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Scene.hpp"
#include "ShaderRef.hpp"
#include "Stopwatch.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("FPS", birb::vec2<i32>(1280, 720));
	window.hot_reload_assets_on_focus_change = true;
	window.init_imgui();
	window.lock_cursor_to_window();

	birb::timestep timestep;
	birb::scene scene;

	birb::renderer renderer;
	renderer.set_scene(scene);
	renderer.debug.alloc_world(window);

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::camera camera;
	camera.far_clip = 100;

	/////////////
	// Shaders //
	/////////////
	birb::shader_ref default_color_shader("default", "default");

	//////////////////////
	// Set the world up //
	//////////////////////

	birb::shader::directional_direction = { 0.76, -1.54, 0.96 };

	camera.position.y = 4;
	camera.fov = 75;
	camera.mode = birb::camera::mode::fps;

	birb::entity world = scene.create_entity();

	birb::stopwatch model_loading("Model loading");
	birb::model world_model("world.obj");
	birb::model tree_model("tree.obj");
	model_loading.stop();

	birb::random rng;

	std::array<std::unique_ptr<birb::entity>, 20> trees;
	constexpr float tree_area = 50.f;
	for (size_t i = 0; i < trees.size(); ++i)
	{
		trees.at(i) = std::make_unique<birb::entity>(scene.create_entity());

		birb::entity& tree = *trees.at(i);
		tree.add_component(tree_model);

		birb::transform transform;
		transform.position.x = rng.range_float(-tree_area, tree_area);
		transform.position.y = -0.5f;
		transform.position.z = rng.range_float(-tree_area, tree_area);
		transform.rotation.y = rng.range_float(0.0f, 360.0f);
		transform.local_scale = 3.0f;
		transform.lock();
		tree.add_component(transform);

		tree.add_component(default_color_shader);
	}

	birb::transform transform;
	transform.position.y = -0.5f;

	world.add_component(world_model);
	world.add_component(transform);
	world.add_component(default_color_shader);


	while (!window.should_close())
	{
		camera.process_input(window, timestep);

		// for (size_t i = 0; i < trees.size(); ++i)
		// 	trees[i]->get_component<birb::transform>().rotation.y += timestep.deltatime() * 5.0f;

		window.clear();

		renderer.draw_entities(camera, window.size());

		performance_overlay.draw();
		renderer_overlay.draw();

		window.flip();
		window.poll();
		timestep.step();
	}
}
