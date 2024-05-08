#pragma once

#include "World.hpp"

#include <memory>

namespace birb
{
	class world;
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
		void alloc_world(window& window);

	private:
		std::unique_ptr<birb::world> world;
	};
}
