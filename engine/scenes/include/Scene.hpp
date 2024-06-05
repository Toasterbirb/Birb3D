#pragma once

#include "Types.hpp"

#include <entt.hpp>
#include <string>

namespace birb
{
	class entity;

	/**
	 * @brief Component flags to be used with scene::create_entity(u32 components)
	 */
	struct component
	{
		constexpr static inline u32 transform 		= 1 << 0;
		constexpr static inline u32 transformer		= 1 << 1;
		constexpr static inline u32 state 			= 1 << 2;
		constexpr static inline u32 box				= 1 << 3;
		constexpr static inline u32 box2d			= 1 << 4;
		constexpr static inline u32 rigidbody		= 1 << 5;
		constexpr static inline u32 default_shader	= 1 << 6;
	};

	class scene
	{
	public:
		scene();
		~scene();
		scene(scene&) = delete;
		scene(const scene&) = delete;
		scene(const scene&&) = delete;

		entt::registry registry;

		/**
		 * @brief Name of the scene
		 */
		std::string name = "Scene";

		birb::entity create_entity();
		birb::entity create_entity(const u32 components);
		void destroy_entity(const entt::entity& entity);
		bool is_duplicate_entity_info_name(const std::string& name, const entt::entity& ignored_entity = entt::null);
		bool is_entity_active(const entt::entity& entity) const;

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
