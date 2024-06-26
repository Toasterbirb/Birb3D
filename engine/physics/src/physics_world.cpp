#include "Assert.hpp"
#include "BoxCollider.hpp"
#include "Entity.hpp"
#include "GravityForce.hpp"
#include "PhysicsWorld.hpp"
#include "Profiling.hpp"
#include "Rigidbody.hpp"
#include "Scene.hpp"
#include "State.hpp"
#include "Transform.hpp"

namespace birb
{
	physics_world::physics_world() {}

	void physics_world::set_scene(scene& scene)
	{
		ensure(scene::scene_count() > 0);
		current_scene = &scene;
	}

	void physics_world::tick(f64 deltatime)
	{
		PROFILER_SCOPE_PHYSICS_FN();

		ensure(current_scene != nullptr, "Current scene has not been set");

		update_rigidbodies(deltatime);
	}

	void physics_world::update_rigidbodies(const f64 deltatime)
	{
		PROFILER_SCOPE_PHYSICS_FN();

		entt::registry& registry = current_scene->registry;

		const auto view = registry.view<rigidbody, transform>();
		for (const auto& entity : view)
		{
			rigidbody& rigidbody = view.get<birb::rigidbody>(entity);
			transform& transform = view.get<birb::transform>(entity);

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

	std::unordered_set<entt::entity> physics_world::collides_with(const birb::entity& entity)
	{
		return collides_with(entity.entt());
	}

	std::unordered_set<entt::entity> physics_world::collides_with(const entt::entity& entity)
	{
		ensure(current_scene != nullptr, "Current scene has not been set");

		std::unordered_set<entt::entity> colliding_entities;

		entt::registry& registry = current_scene->registry;
		ensure(registry.try_get<collider::box>(entity), "Tried to check collision with an entity that doesn't have a box collider on it");
		collider::box& target_collider = registry.get<collider::box>(entity);

		const auto view = registry.view<collider::box>();
		for (const auto& collider_entity : view)
		{
			// The entity shouldn't collide with itself
			if (entity == collider_entity)
				continue;

			// Skip the entity if it's state is set to disabled
			const birb::state* state = registry.try_get<birb::state>(collider_entity);
			if (state && !state->active)
				continue;

			if (target_collider.collides_with(view.get<collider::box>(collider_entity)))
				colliding_entities.insert(collider_entity);
		}

		return colliding_entities;
	}
}
