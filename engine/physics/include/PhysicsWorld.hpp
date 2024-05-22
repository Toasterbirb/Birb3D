#pragma once

#include <entt.hpp>
#include <unordered_set>

namespace birb
{
	class entity;
	class scene;

	class physics_world
	{
	public:
		physics_world();
		~physics_world() = default;
		physics_world(const physics_world&) = default;
		physics_world(physics_world&) = default;

		void set_scene(scene& scene);
		void tick(const f64 deltatime);

		std::unordered_set<entt::entity> collides_with(const birb::entity& entity);
		std::unordered_set<entt::entity> collides_with(const entt::entity& entity);

	private:
		void update_rigidbodies(const f64 deltatime);

		scene* current_scene = nullptr;
	};
}
