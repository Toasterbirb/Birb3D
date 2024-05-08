#pragma once

#include "Inspector.hpp"
#include "WidgetBase.hpp"

#include <entt.hpp>

namespace birb
{
	class scene;

	class entity_editor : public widget_base
	{
	public:
		explicit entity_editor(birb::scene& scene);
		~entity_editor() = default;
		entity_editor(const entity_editor &) = default;
		entity_editor(entity_editor &) = default;

		void draw() override;

	private:
		birb::scene& scene;
		entt::entity selected_entity = entt::null;

		birb::inspector inspector;
	};
}
