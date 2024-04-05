#include "Color.hpp"
#include "Vector.hpp"

namespace birb
{
	vec3<float> color::to_vec3()
	{
		return vec3<float>(r, g, b);
	}
}
