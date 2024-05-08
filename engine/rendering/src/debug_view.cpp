#include "DebugView.hpp"
#include "Globals.hpp"

namespace birb
{
	debug_view::debug_view()
	{

	}

	void debug_view::draw()
	{
		assert(imgui_initialized && "Debug view requires ImGui to be initialized");

		// Draw widgets that have been constructed

		if (world.get())
			world->draw();
	}

	void debug_view::alloc_world(window& window)
	{
		assert(!world.get() && "World widget has already been allocated");
		world = std::make_unique<birb::world>(window);
	}
}
