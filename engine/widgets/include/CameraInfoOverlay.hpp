#pragma once

#include "Camera.hpp"
#include "WidgetBase.hpp"

namespace birb
{
	namespace overlay
	{
		class camera_info : public widget_base
		{
		public:
			explicit camera_info(const camera& camera);

			void draw() override;

		private:
			const camera& camera_obj;
		};
	}
}
