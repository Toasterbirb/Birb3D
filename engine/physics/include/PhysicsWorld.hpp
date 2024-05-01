#pragma once

#include <entt.hpp>
#include <vector>

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
		void tick(f64 deltatime);

		std::vector<entt::entity> collides_with(const birb::entity& entity);
		std::vector<entt::entity> collides_with(const entt::entity& entity);

	private:
		scene* current_scene = nullptr;

	};
}
