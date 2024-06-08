#include "Assert.hpp"
#include "Box2DCollider.hpp"
#include "BoxCollider.hpp"
#include "Entity.hpp"
#include "Info.hpp"
#include "Logger.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Rigidbody.hpp"
#include "Scene.hpp"
#include "ShaderRef.hpp"
#include "State.hpp"
#include "Transform.hpp"
#include "Transformer.hpp"

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

	birb::entity scene::create_entity(const u32 components)
	{
		ensure(components != 0, "Use the create_entity() function that takes no arguments if you don't need any components");

		entt::entity entt_entity = registry.create();

		if (components & component::transform)
			add_component(entt_entity, transform());

		if (components & component::transformer)
			add_component(entt_entity, transformer());

		if (components & component::state)
			add_component(entt_entity, state());

		if (components & component::box)
			add_component(entt_entity, collider::box());

		if (components & component::box2d)
			add_component(entt_entity, collider::box2d());

		if (components & component::rigidbody)
		{
			ensure(components & component::transform, "Rigidbody needs a transform component");
			add_component(entt_entity, rigidbody(get_component<transform>(entt_entity)));
		}

		if (components & component::default_shader)
			add_component(entt_entity, shader_ref("default", "default"));

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

	bool scene::is_entity_active(const entt::entity& entity) const
	{
		const birb::state* state = registry.try_get<birb::state>(entity);
		if (state)
			return state->active;

		return true;
	}

	bool scene::is_entity_renderable(const entt::entity& entity) const
	{
		const birb::state* state = registry.try_get<birb::state>(entity);
		if (state)
			return state->should_be_rendered();

		return true;
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
