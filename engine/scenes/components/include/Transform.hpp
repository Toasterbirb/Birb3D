#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"

namespace birb
{
	namespace component
	{
		class transform : public editor_component
		{
		public:
			transform();

			void draw_editor_ui() override;

			vec3<float> position;
			vec3<float> rotation;
			vec3<float> local_scale;

		private:

		};
	}
}
