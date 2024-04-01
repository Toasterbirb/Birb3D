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
		u32 counter = 0;
		birb::random rng;

		entt::entity default_entity(std::string name);
		static inline bool duplicate_name = false;

		// Cache some shader stuff that won't change during runtime
		std::string vertex_shader_name_list_str;
		std::string fragment_shader_name_list_str;
		const std::string default_vert_shader_name_str = "default";
		const std::string default_frag_shader_name_str = "default_color";
		u16 default_vertex_shader_index = 0;
		u16 default_fragment_shader_index = 0;
	};
}
