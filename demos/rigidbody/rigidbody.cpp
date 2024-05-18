#include "BoxCollider.hpp"
#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "GravityForce.hpp"
#include "Logger.hpp"
#include "PhysicsWorld.hpp"
#include "Renderer.hpp"
#include "Rigidbody.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "Timestep.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Physics", { 1280, 720 });
	window.init_imgui();
	birb::timestep timestep;

	birb::renderer renderer;
	birb::scene scene;
	renderer.set_scene(scene);

	birb::camera camera;
	camera.position.x = -128;
	camera.position.z = 45;
	camera.orthograhpic_scale = 0.5;

	birb::physics_world world;
	world.set_scene(scene);

	const std::string cube_path = "texture_512.png";
	const std::string floor_path = "wide_texture_512x256.png";

	birb::sprite cube_sprite(cube_path, birb::color_format::RGB);
	birb::sprite floor_sprite(floor_path, birb::color_format::RGB);
	floor_sprite.ignore_aspect_ratio = true;

	birb::entity cube = scene.create_entity();
	cube.add_component(cube_sprite);
	{
		birb::transform transform;
		transform.position = { 128.0f, 256.0f, 0.0f };
		transform.local_scale = { 32.0f, 32.0f, 1.0f };
		cube.add_component(transform);

		birb::rigidbody rigidbody(transform);
		rigidbody.set_mass(10);
		cube.add_component(rigidbody);

		birb::physics_forces::gravity gravity;
		cube.add_component(gravity);

		birb::collider::box box;
		box.set_position_and_size(transform);
		cube.add_component(box);
	}

	birb::entity floor = scene.create_entity();
	floor.add_component(floor_sprite);
	{
		birb::transform transform;
		transform.position = { 128.0f, 64.0f, 0.0f };
		transform.local_scale = floor_sprite.texture->size().to_float() / 4.0f;
		floor.add_component(transform);

		birb::collider::box box;
		box.set_position_and_size(transform);
		floor.add_component(box);
	}

	i32 counter = 0;
	while (!window.should_close())
	{
		world.tick(timestep.deltatime());

		std::vector<entt::entity> collisions = world.collides_with(cube);
		if (!collisions.empty())
			birb::log("Collision: ", counter++);

		window.clear();
		renderer.draw_entities(camera, window.size());
		window.flip();

		window.poll();
		timestep.step();
	}

	return 0;
}
