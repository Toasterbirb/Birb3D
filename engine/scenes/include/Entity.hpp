#pragma once

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
		void add_component(const T& component)
		{
			static_assert(!std::is_same<T, birb::entity>::value, "You shouldn't need to use birb::entity as a component");
			parent_scene->add_component(entt_entity, component);
		}

		template<typename T>
		T& get_component()
		{
			return parent_scene->get_component<T>(entt_entity);
		}

		entt::entity entt() const;

	private:
		scene* parent_scene;
		entt::entity entt_entity;
	};
}
