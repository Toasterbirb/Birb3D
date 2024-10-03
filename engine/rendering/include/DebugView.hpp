#pragma once

#include "CameraInfoOverlay.hpp"
#include "EntityEditor.hpp"
#include "PerformanceOverlay.hpp"
#include "RendererOverlay.hpp"
#include "World.hpp"

#include <memory>

namespace birb
{
	class camera;
	class renderer;
	class scene;
	class timestep;
	class window;

	class debug_view
	{
	public:
		debug_view();
		~debug_view() = default;
		debug_view(const debug_view&) = delete;
		debug_view(debug_view&) = delete;

		void draw_widgets();
		void draw_overlays();

		// Widget allocators
		void alloc_entity_editor(scene& scene);
		void alloc_world(window& window);

		// Overlay allocators
		void alloc_render_stats(renderer& renderer);
		void alloc_performance_stats(timestep& timestep);
		void alloc_camera_info(camera& camera);

	private:
		// Widgets
		std::unique_ptr<birb::world> world;
		std::unique_ptr<birb::entity_editor> entity_inspector;

		// Overlays
		std::unique_ptr<birb::overlay::performance> performance_stats;
		std::unique_ptr<birb::overlay::renderer_overlay> render_stats;
		std::unique_ptr<birb::overlay::camera_info> camera_info;

		// References to things shared between widgets and overlays
		camera* camera_ptr{nullptr};

		ImGuiID dockspace_id;
		void setup_docking();
		bool is_docking_setup{false};
	};
}
