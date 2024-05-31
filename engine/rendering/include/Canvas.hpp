#pragma once

#include "ShaderRef.hpp"
#include "UIBase.hpp"

#include <vector>

namespace birb
{
	namespace ui
	{
		class canvas
		{
		public:
			canvas();
			~canvas() = default;
			canvas(const canvas&) = default;
			canvas(canvas&) = default;

			void add_ui_element(ui_base& ui_element);

			void draw();

		private:
			std::vector<ui_base*> ui_elements;
			shader_ref ui_shader;
		};
	}
}
