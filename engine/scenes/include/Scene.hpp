#pragma once

#include <entt.hpp>

namespace birb
{
	class entity;

	class scene
	{
	public:
		scene();
		~scene();

		entt::registry& get_registry();

		birb::entity create_entity();

		template<typename T>
		void add_component(entt::entity& entity, const T& component)
		{
			registry.emplace<T>(entity, component);
		}

		template<typename T>
		T& get_component(const entt::entity& entity)
		{
			return registry.get<T>(entity);
		}

	private:
		entt::registry registry;
	};
}
