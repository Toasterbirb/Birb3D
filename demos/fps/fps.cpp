#include "BoxCollider.hpp"
#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "GravityForce.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "PhysicsWorld.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "Rigidbody.hpp"
#include "Scene.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "ShaderSprite.hpp"
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
	renderer.opt_blend(true);
	renderer.set_scene(scene);
	renderer.debug.alloc_world(window);
	renderer.debug.alloc_entity_editor(scene);

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::camera camera(window.size());
	camera.far_clip = 100;
	renderer.debug.alloc_camera_info(camera);

	/////////////
	// Shaders //
	/////////////

	birb::shader_ref default_color_shader("default", "default");

	//////////////////////
	// Set the world up //
	//////////////////////

	birb::physics_world physics_world;
	physics_world.set_scene(scene);

	birb::shader::directional_light.direction = { 0.76, -1.54, 0.96 };

	birb::entity player = scene.create_entity("Player", birb::component::transform | birb::component::box | birb::component::rigidbody);
	player.get_component<birb::transform>().local_scale = { 1.0f, 1.0f, 1.0f };
	player.get_component<birb::collider::box>().set_size(player.get_component<birb::transform>().local_scale / 2.0f);
	player.add_component<birb::physics_forces::gravity>(birb::physics_forces::gravity());
	player.get_component<birb::rigidbody>().set_mass(10.0f);

	camera.position.y = 4.0f;
	camera.fov = 75;
	camera.mode = birb::camera::mode::fps;

	birb::entity floor = scene.create_entity("Floor", birb::component::transform | birb::component::box);
	floor.get_component<birb::transform>().position.y = -0.5f;
	floor.get_component<birb::transform>().local_scale = { 40.0f, 0.5f, 40.0f };
	floor.get_component<birb::collider::box>().set_position_and_size(floor.get_component<birb::transform>());

	birb::entity world = scene.create_entity("World");

	birb::stopwatch model_loading("Model loading");
	birb::model world_model("world.obj");
	birb::model tree_model("tree.obj");
	model_loading.stop();

	birb::random rng;

	std::array<std::unique_ptr<birb::entity>, 100> trees;
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

	birb::shader::shader_src_search_paths.push_back("shaders");
	birb::entity shader_sprite = scene.create_entity("Shader sprite", birb::component::transform);

	shader_sprite.get_component<birb::transform>().position = floor.get_component<birb::transform>().position;
	shader_sprite.get_component<birb::transform>().position.y += floor.get_component<birb::transform>().local_scale.y / 2.0 + 0.01;

	shader_sprite.get_component<birb::transform>().rotation = { -90, 0, 0 };
	const f32 floor_width = floor.get_component<birb::transform>().local_scale.x;
	shader_sprite.get_component<birb::transform>().local_scale = { floor_width, floor_width, floor_width};
	// shader_sprite.get_component<birb::transform>().position = { 0, 4, 16 };
	// shader_sprite.get_component<birb::transform>().local_scale = { 8, 8, 1 };
	// shader_sprite.get_component<birb::transform>().rotation.y = 180;
	birb::shader_sprite sprite_s("fancy_effect");
	sprite_s.orthographic_projection = false;
	shader_sprite.add_component(sprite_s);

	f32 time{};
	birb::color shader_color = 0xDC6D6D;

	while (!window.should_close())
	{
		time += timestep.deltatime();

		camera.process_input(window, timestep);

		player.get_component<birb::rigidbody>().position = { camera.position.x, camera.position.y - 4, camera.position.z };
		physics_world.tick(timestep.deltatime());

		birb::vec3<f32> camera_position = player.get_component<birb::transform>().position;
		camera_position.y += 4.0f;
		camera.position = camera_position;

		// birb::log("Position: ", player.get_component<birb::transform>().position, " | Force: ", player.get_component<birb::rigidbody>().velocity);

		// Handle the floor collision
		std::unordered_set<entt::entity> player_collisions = physics_world.collides_with(player);
		if (player_collisions.contains(floor.entt()))
		{
			// birb::log("Collision!");
			player.get_component<birb::rigidbody>().position.y = 0.0f;
			player.get_component<birb::rigidbody>().add_force({ 0.0f, 98.1, 0.0f });
		}

		// Update the shader sprite
		{
			std::shared_ptr<birb::shader> shader = birb::shader_collection::get_shader(shader_sprite.get_component<birb::shader_sprite>().shader_reference());
			shader->activate();
			shader->set("time", time);
			shader->set("color", shader_color);
		}

		window.clear();

		renderer.draw_entities(camera, window.size());

		performance_overlay.draw();
		renderer_overlay.draw();

		window.flip();
		window.poll();
		timestep.step();
	}
}
