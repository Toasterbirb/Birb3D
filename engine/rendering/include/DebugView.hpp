#pragma once

#include "World.hpp"
#include "EntityEditor.hpp"

#include <memory>

namespace birb
{
	class scene;
	class window;

	class debug_view
	{
	public:
		debug_view();
		~debug_view() = default;
		debug_view(const debug_view&) = delete;
		debug_view(debug_view&) = delete;

		void draw();

		// Widget allocators
		void alloc_entity_editor(scene& scene);
		void alloc_world(window& window);

	private:
		std::unique_ptr<birb::world> world;
		std::unique_ptr<birb::entity_editor> entity_inspector;
	};
}
