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

	u64 shader_ref::hash() const
	{
		return combined_hash;
	}

	u64 shader_ref::vertex() const
	{
		return vertex_hash;
	}

	u64 shader_ref::fragment() const
	{
		return fragment_hash;
	}

	void shader_ref::update_hashes(const std::string& vertex, const std::string& fragment)
	{
		ensure(!vertex.empty());
		ensure(!fragment.empty());

		this->vertex_hash = std::hash<std::string>{}(vertex);
		this->fragment_hash = std::hash<std::string>{}(fragment);

		// Some hash combination thing found from cppreference
		combined_hash = combine_hashes(this->vertex_hash, this->fragment_hash);
	}
}
