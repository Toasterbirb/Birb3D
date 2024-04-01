#include "GravityForce.hpp"
#include "PhysicsWorld.hpp"
#include "Rigidbody.hpp"
#include "Transform.hpp"

namespace birb
{
	physics_world::physics_world() {}

	void physics_world::set_scene(scene& scene)
	{
		assert(scene::scene_count > 0);
		current_scene = &scene;
	}

	void physics_world::tick(double deltatime)
	{
		entt::registry& registry = current_scene->registry;

		// Get all rigidbodies
		const auto view = registry.view<rigidbody, component::transform>();
		for (const auto& entity : view)
		{
			rigidbody& rigidbody = view.get<birb::rigidbody>(entity);
			component::transform& transform = view.get<birb::component::transform>(entity);

			// Apply gravity force
			physics_forces::gravity* gravity_force = registry.try_get<physics_forces::gravity>(entity);
			if (gravity_force)
				gravity_force->update_force(rigidbody);

			// Update the position and velocity stuff
			rigidbody.update(deltatime);
			transform.position = rigidbody.position;
		}
	}
}
