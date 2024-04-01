#pragma once

#include "Scene.hpp"

namespace birb
{
	class physics_world
	{
	public:
		physics_world();
		~physics_world() = default;
		physics_world(const physics_world&) = default;
		physics_world(physics_world&) = default;

		void set_scene(scene& scene);
		void tick(double deltatime);

	private:
		scene* current_scene;

	};
}
