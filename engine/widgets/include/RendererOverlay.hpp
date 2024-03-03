#pragma once

#include "Renderer.hpp"
#include "WidgetBase.hpp"

namespace birb
{
	namespace overlay
	{
		class renderer_overlay : public widget_base
		{
		public:
			explicit renderer_overlay(const birb::renderer& renderer);

			void draw() override;

		private:
			const birb::renderer& renderer;
		};
	}
}
