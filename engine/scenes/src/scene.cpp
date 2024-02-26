#include "entt.hpp"

#include "Entity.hpp"
#include "Scene.hpp"

namespace birb
{
	scene::scene()
	{

	}

	scene::~scene()
	{

	}

	entt::registry& scene::get_registry()
	{
		return registry;
	}

	birb::entity scene::create_entity()
	{
		entt::entity entt_entity = registry.create();
		return birb::entity(this, entt_entity);
	}
}
