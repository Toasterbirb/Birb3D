#include "Entity.hpp"

#include <cassert>

namespace birb
{
	entity::entity(scene* scene, const entt::entity& entt_entity)
	:parent_scene(scene), entt_entity(entt_entity)
	{
		assert(scene != nullptr);
	}

	entity::~entity()
	{

	}
}
