#pragma once

#include "WidgetBase.hpp"

namespace birb
{
	class camera;

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
