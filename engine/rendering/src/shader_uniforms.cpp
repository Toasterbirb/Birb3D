#include "ShaderUniforms.hpp"

#include <cassert>

namespace birb
{
	std::string uniform::str(i32 index) const
	{
		std::string str = name;

		assert((!is_array && index == -1) || (is_array && index >= 0));

		if (is_array)
			str += "[" + std::to_string(index) + "]";

		if (!struct_var.empty())
			str += "." + struct_var;

		return str;
	}
}
