#pragma once

#include "Vector.hpp"

namespace birb
{
	namespace component
	{
		class transform
		{
		public:
			transform();

			vec3<float> position;
			vec3<float> rotation;
			vec3<float> local_scale;

		private:

		};
	}
}
