#include "BoxCollider.hpp"
#include "GravityForce.hpp"
#include "PhysicsWorld.hpp"
#include "Rigidbody.hpp"
#include "Transform.hpp"

namespace birb
{
	physics_world::physics_world() {}

	void physics_world::set_scene(scene& scene)
	{
		assert(scene::scene_count() > 0);
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

			// Update colliders
			collider::box* box = registry.try_get<collider::box>(entity);
			if (box)
				box->set_position(transform.position);
		}
	}

	std::vector<entt::entity> physics_world::collides_with(const birb::entity& entity)
	{
		return collides_with(entity.entt());
	}

	std::vector<entt::entity> physics_world::collides_with(const entt::entity& entity)
	{
		std::vector<entt::entity> colliding_entities;

		entt::registry& registry = current_scene->registry;
		assert(registry.try_get<collider::box>(entity) && "Tried to check collision with an entity that doesn't have a box collider on it");
		collider::box& target_collider = registry.get<collider::box>(entity);

		const auto view = registry.view<collider::box>();
		for (const auto& collider_entity : view)
		{
			// The entity shouldn't collide with itself
			if (entity == collider_entity)
				continue;

			if (target_collider.collides_with(view.get<collider::box>(collider_entity)))
				colliding_entities.push_back(collider_entity);
		}

		return colliding_entities;
	}
}
