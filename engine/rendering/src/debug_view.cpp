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

	void debug_view::draw_widgets()
	{
#ifndef BIRB_RELEASE
		ensure(g_imgui_initialized, "Debug view requires ImGui to be initialized");

		// Widgets
		if (entity_inspector.get())
			entity_inspector->draw();

		if (world.get())
			world->draw();
#endif
	}

	void debug_view::draw_overlays()
	{
#ifndef BIRB_RELEASE
		ensure(g_imgui_initialized, "Debug view requires ImGui to be initialized");

		// Overlays
		if (performance_stats.get())
			performance_stats->draw();

		if (render_stats.get())
			render_stats->draw();

		if (camera_info.get())
			camera_info->draw();
#endif
	}

	void debug_view::alloc_entity_editor(scene& scene)
	{
#ifndef BIRB_RELEASE
		ensure(!entity_inspector.get(), "Entity inspector widget has already been allocated");
		entity_inspector = std::make_unique<birb::entity_editor>(scene);
#endif
	}

	void debug_view::alloc_world(window& window)
	{
#ifndef BIRB_RELEASE
		ensure(!world.get(), "World widget has already been allocated");
		world = std::make_unique<birb::world>(window);
#endif
	}

	void debug_view::alloc_render_stats(renderer& renderer)
	{
#ifndef BIRB_RELEASE
		ensure(!render_stats.get(), "Render stats has already been allocated");
		render_stats = std::make_unique<birb::overlay::renderer_overlay>(renderer);
#endif
	}

	void debug_view::alloc_performance_stats(timestep& timestep)
	{
#ifndef BIRB_RELEASE
		ensure(!performance_stats.get(), "Performance stats has already been allocated");
		performance_stats = std::make_unique<birb::overlay::performance>(timestep);
#endif
	}

	void debug_view::alloc_camera_info(camera& camera)
	{
#ifndef BIRB_RELEASE
		ensure(!camera_info.get(), "Camera info has already been allocated");
		camera_info = std::make_unique<birb::overlay::camera_info>(camera);
#endif
	}
}
