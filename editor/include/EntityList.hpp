#pragma once

#include "EditorWindow.hpp"
#include "Scene.hpp"

namespace editor
{
	class entity_list : public editor_window
	{
	public:
		explicit entity_list(birb::scene& scene);
		void draw() override;

		inline static entt::entity selected_entity = entt::null;
		inline static const std::string& new_entity_button_text = "New entity";

	private:
		birb::scene& scene;
		unsigned int counter = 0;
	};
}
