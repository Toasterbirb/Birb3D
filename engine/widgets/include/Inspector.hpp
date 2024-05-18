#pragma once

#include "WidgetBase.hpp"
#include "Scene.hpp"

#include <entt.hpp>

namespace birb
{
	class inspector : public widget_base
	{
	public:
		explicit inspector(birb::scene& scene);

		void set_selected(entt::entity& entity);


		/**
		 * @brief Draw editor specific stuff
		 */
		bool editor_mode = false;

		void draw() override;

	private:
		birb::scene& scene;
		entt::entity selected_entity;

		enum class component_type
		{
			transform,
			material,
		};

		static inline const std::vector<std::pair<std::string, component_type>> component_names = {
			{ "Transform", component_type::transform },
			{ "Material", component_type::material },
		};

		static inline std::string component_type_str = "";
	};
}
