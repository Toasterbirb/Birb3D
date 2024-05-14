#include "Assert.hpp"
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
		ensure(imgui_initialized && "Debug view requires ImGui to be initialized");

		if (entity_inspector.get())
			entity_inspector->draw();

		if (world.get())
			world->draw();
	}

	void debug_view::alloc_entity_editor(scene& scene)
	{
		ensure(!entity_inspector.get() && "Entity inspector widget has already been allocated");
		entity_inspector = std::make_unique<birb::entity_editor>(scene);
	}

	void debug_view::alloc_world(window& window)
	{
		ensure(!world.get() && "World widget has already been allocated");
		world = std::make_unique<birb::world>(window);
	}
}
