#include "DebugView.hpp"
#include "Globals.hpp"
#include "Scene.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace birb
{
	debug_view::debug_view()
	{
	}

	void debug_view::draw()
	{
		assert(imgui_initialized && "Debug view requires ImGui to be initialized");

		if (entity_inspector.get())
			entity_inspector->draw();

		if (world.get())
			world->draw();
	}

	void debug_view::alloc_entity_editor(scene& scene)
	{
		assert(!entity_inspector.get() && "Entity inspector widget has already been allocated");
		entity_inspector = std::make_unique<birb::entity_editor>(scene);
	}

	void debug_view::alloc_world(window& window)
	{
		assert(!world.get() && "World widget has already been allocated");
		world = std::make_unique<birb::world>(window);
	}
}
