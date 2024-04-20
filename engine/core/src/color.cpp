#include "Color.hpp"
#include "Vector.hpp"

namespace birb
{
	vec3<f32> color::to_vec3()
	{
		return vec3<f32>(r, g, b);
	}
}
