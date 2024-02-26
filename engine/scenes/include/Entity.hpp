#pragma once

#include "Scene.hpp"

#include <entt.hpp>

namespace birb
{
	class entity
	{
	public:
		explicit entity(scene* scene, const entt::entity& entity);
		~entity();

		template<typename T>
		void add_component(const T& component)
		{
			parent_scene->add_component(entt_entity, component);
		}

		template<typename T>
		T& get_component()
		{
			return parent_scene->get_component<T>(entt_entity);
		}

	private:
		scene* parent_scene;
		entt::entity entt_entity;
	};
}
