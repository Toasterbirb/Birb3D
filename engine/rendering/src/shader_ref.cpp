#include "Assert.hpp"
#include "Math.hpp"
#include "ShaderRef.hpp"

namespace birb
{
	shader_ref::shader_ref(const std::string& shader_name)
	{
		update_hashes(shader_name, shader_name);
	}

	shader_ref::shader_ref(const std::string& vertex, const std::string& fragment)
	{
		update_hashes(vertex, fragment);
	}

	void shader_ref::update_hashes(const std::string& vertex, const std::string& fragment)
	{
		ensure(!vertex.empty());
		ensure(!fragment.empty());

		this->vertex = std::hash<std::string>{}(vertex);
		this->fragment = std::hash<std::string>{}(fragment);

		// Some hash combination thing found from cppreference
		hash = combine_hashes(this->vertex, this->fragment);
	}
}
