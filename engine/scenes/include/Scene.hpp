#pragma once

#include "Types.hpp"

#include <entt.hpp>
#include <string>

namespace birb
{
	class entity;

	enum class entity_template
	{
		gameobject,				// state, transform
		gameobject_box3d,		// state, transform, 3d box collider
		gameobject_rigidbody,	// state, transform, 3d box collider, rigidbody
	};

	class scene
	{
	public:
		scene();
		~scene();

		entt::registry registry;

		/**
		 * @brief Name of the scene
		 */
		std::string name = "Scene";

		birb::entity create_entity();
		birb::entity create_entity(const entity_template entity_template);
		void destroy_entity(const entt::entity& entity);
		bool is_duplicate_entity_info_name(const std::string& name, const entt::entity& ignored_entity = entt::null);

		template<typename T>
		void add_component(entt::entity& entity, const T& component)
		{
			static_assert(!std::is_same<T, birb::entity>::value, "birb::entity shouldn't be used as a component");
			registry.emplace<T>(entity, component);
		}

		template<typename T>
		void remove_component(entt::entity& entity)
		{
			registry.remove<T>(entity);
		}

		template<typename T>
		T& get_component(const entt::entity& entity)
		{
			return registry.get<T>(entity);
		}

		void reload_models();

		static i16 scene_count();

	private:
	};
}
