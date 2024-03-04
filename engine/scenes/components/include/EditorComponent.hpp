#pragma once

#include <entt.hpp>

namespace birb
{
	/**
	 * @class editor_component
	 * @brief Interface for adding editor UI to entity components
	 *
	 */
	class editor_component
	{
	public:
		template<typename T>
		static void try_draw_ui(entt::registry& registry, const entt::entity& entity)
		{
			if (T* component = registry.try_get<T>(entity))
				component->draw_editor_ui();
		}

		virtual void draw_editor_ui() = 0;
	};
}
