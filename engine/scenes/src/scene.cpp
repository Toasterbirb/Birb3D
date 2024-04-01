#include "entt.hpp"

#include "Entity.hpp"
#include "Info.hpp"
#include "Scene.hpp"
#include "Logger.hpp"

namespace birb
{
	static i16 current_scene_count = 0;

	scene::scene()
	{
		assert(current_scene_count >= 0);
		current_scene_count++;
	}

	scene::~scene()
	{
		current_scene_count--;
	}

	birb::entity scene::create_entity()
	{
		entt::entity entt_entity = registry.create();
		return birb::entity(this, entt_entity);
	}

	bool scene::is_duplicate_entity_info_name(const std::string& name, const entt::entity& ignored_entity)
	{
		assert(!name.empty());

		const auto view = registry.view<birb::component::info>();

		// Default check that doesn't ignore any entities
		if (ignored_entity == entt::null)
		{
			return std::any_of(view.begin(), view.end(), [name, view](const auto& entity){
				return name == view.get<birb::component::info>(entity).name;
			});
		}
		// Ignore an entity while checking for duplicates
		else
		{
			return std::any_of(view.begin(), view.end(), [name, view, ignored_entity](const auto& entity){
				return name == view.get<birb::component::info>(entity).name && entity != ignored_entity;
			});
		}

		return false;
	}

	i16 scene::scene_count()
	{
		return current_scene_count;
	}
}
