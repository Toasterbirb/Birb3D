#include "Assert.hpp"
#include "Entity.hpp"
#include "Info.hpp"
#include "Logger.hpp"
#include "Model.hpp"
#include "Scene.hpp"

#include <entt.hpp>

namespace birb
{
	static i16 current_scene_count = 0;

	scene::scene()
	{
		ensure(current_scene_count >= 0);
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

	void scene::destroy_entity(const entt::entity& entity)
	{
		ensure(registry.valid(entity), "Attempted to destroy an invalid entity");
		registry.destroy(entity);
	}

	bool scene::is_duplicate_entity_info_name(const std::string& name, const entt::entity& ignored_entity)
	{
		ensure(!name.empty());

		const auto view = registry.view<birb::info>();

		// Default check that doesn't ignore any entities
		if (ignored_entity == entt::null)
		{
			return std::any_of(view.begin(), view.end(), [name, view](const auto& entity){
				return name == view.get<birb::info>(entity).name;
			});
		}
		// Ignore an entity while checking for duplicates
		else
		{
			return std::any_of(view.begin(), view.end(), [name, view, ignored_entity](const auto& entity){
				return name == view.get<birb::info>(entity).name && entity != ignored_entity;
			});
		}

		return false;
	}

	void scene::reload_models()
	{
		const auto view = registry.view<birb::model>();

		for (auto& entity : view)
		{
			model& model = view.get<birb::model>(entity);
			model.reload();
		}
	}

	i16 scene::scene_count()
	{
		return current_scene_count;
	}
}
