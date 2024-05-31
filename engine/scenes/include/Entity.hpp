#pragma once

#include "Assert.hpp"
#include "Scene.hpp"

#include <entt.hpp>
#include <type_traits>

namespace birb
{
	class entity
	{
	public:
		explicit entity(scene* scene, const entt::entity& entity);
		~entity();

		template<typename T>
		constexpr void add_component(const T& component)
		{
			static_assert(!std::is_same<T, birb::entity>::value, "You shouldn't need to use birb::entity as a component");
			static_assert(!std::is_same<T, entt::entity>::value, "You shouldn't need to use entt::entity as a component");
			ensure(!has_component<T>(), "An entity can only have a singular instance of each component type");
			parent_scene->add_component(entt_entity, component);
		}

		template<class... component>
		constexpr void add_components(component... components)
		{
			(add_component(components), ...);
		}

		template<typename T>
		constexpr T& get_component()
		{
			static_assert(!std::is_same<T, birb::entity>::value, "There shouldn't be any situation where birb::entity has a birb::entity as component");
			static_assert(!std::is_same<T, entt::entity>::value, "There shouldn't be any situation where birb::entity has an entt::entity as component");
			ensure(has_component<T>(), "Can't get a reference to a nonexistent component");
			return parent_scene->get_component<T>(entt_entity);
		}

		template<typename T>
		constexpr bool has_component()
		{
			static_assert(!std::is_same<T, birb::entity>::value, "There shouldn't be any situation where birb::entity has a birb::entity as component");
			static_assert(!std::is_same<T, entt::entity>::value, "There shouldn't be any situation where birb::entity has an entt::entity as component");
			return parent_scene->registry.try_get<T>(entt_entity);
		}

		entt::entity entt() const;

	private:
		scene* parent_scene;
		entt::entity entt_entity;
	};
}
