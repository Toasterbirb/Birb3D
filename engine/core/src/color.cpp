#include "Color.hpp"
#include "Vector.hpp"

namespace birb
{
	vec3<f32> color::to_vec3() const
	{
		return vec3<f32>(r, g, b);
	}

	glm::vec4 color::to_vec4() const
	{
		return glm::vec4(r, g, b, a);
	}
}
