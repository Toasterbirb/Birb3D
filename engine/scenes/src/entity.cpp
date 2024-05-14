#include "Assert.hpp"
#include "Entity.hpp"


namespace birb
{
	entity::entity(scene* scene, const entt::entity& entt_entity)
	:parent_scene(scene), entt_entity(entt_entity)
	{
		ensure(scene != nullptr);
	}

	entity::~entity()
	{

	}

	entt::entity entity::entt() const
	{
		return entt_entity;
	}
}
