#include "ColorFormat.hpp"

#include <glad/gl.h>

namespace birb
{
	// Ensure that the color formats are correct
	static_assert(static_cast<i32>(color_format::RGB) == GL_RGB);
	static_assert(static_cast<i32>(color_format::RGBA) == GL_RGBA);
	static_assert(static_cast<i32>(color_format::DEPTH) == GL_DEPTH_COMPONENT);
}
