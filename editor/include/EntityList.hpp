#pragma once

#include "EditorWindow.hpp"
#include "Random.hpp"
#include "Scene.hpp"

#include <entt.hpp>

namespace editor
{
	class entity_list : public editor_window
	{
	public:
		explicit entity_list(birb::scene& scene);
		void draw() override;

		inline static entt::entity selected_entity = entt::null;
		inline static const std::string& new_entity_menu_text = "New entity";

	private:
		birb::scene& scene;
		unsigned int counter = 0;
		birb::random rng;

		entt::entity default_entity(std::string name);
		static inline bool duplicate_name = false;
	};
}
