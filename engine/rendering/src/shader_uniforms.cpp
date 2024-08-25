#include "Assert.hpp"
#include "ShaderUniforms.hpp"

namespace birb
{
	uniform::uniform(const std::string& name, uniform_type type, const std::string& struct_var, bool is_array, i32 array_max_size)
	:name(name), type(type), is_array(is_array), array_max_size(array_max_size)
	{
		ensure((!is_array && array_max_size == 0) || (is_array && array_max_size != 0));

		if (!struct_var.empty() && !is_array)
		{
			this->name += "." + struct_var;
			return;
		}

		// Generate the array uniform name cache if the uniform is an array
		if (is_array)
		{
			array_str_cache = std::vector<std::string>(array_max_size);
			for (i32 i = 0; i < array_max_size; ++i)
				array_str_cache[i] = name + "[" + std::to_string(i) + "]";

			// Handle struct arrays
			if (!struct_var.empty())
				for (i32 i = 0; i < array_max_size; ++i)
					array_str_cache[i] += "." + struct_var;
		}
	}

	std::string uniform::str(i32 index) const
	{
		ensure((!is_array && index == -1) || (is_array && index >= 0));

		if (is_array)
		{
			ensure(index < array_max_size, "Tried to index things beyond the size of the uniform array");
			return array_str_cache[index];
		}

		return name;
	}

	uniform_block::uniform_block(const u32 bind_point, const char* block_name, const size_t size, const gl_usage usage)
	:bind_point(bind_point), block_name(block_name), size(size), usage(usage)
	{}
}
