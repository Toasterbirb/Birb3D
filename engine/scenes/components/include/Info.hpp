#pragma once

#include "EditorComponent.hpp"

#include <string>

namespace birb
{
	namespace component
	{
		class info : public editor_component
		{
		public:
			explicit info(const std::string& name) : name(name) {}
			std::string name;

			void draw_editor_ui() override;

		private:
		};
	}
}
