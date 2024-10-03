#include "Assert.hpp"
#include "Math.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
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

	void shader_ref::draw_editor_ui()
	{
		const std::shared_ptr<shader> shader = shader_collection::get_shader(*this);
		shader->draw_editor_ui();
		ImGui::Spacing();
		ImGui::Text("Hash: %lu", combined_hash);
		ImGui::SetItemTooltip("The combination of the vertex and fragment hashes");

		ImGui::Text("Small hash: %lu", combined_hash % small_hash_modulo);
		ImGui::SetItemTooltip("Modulo %d of the full hash. Can collide with other small hashes.", small_hash_modulo);
	}

	std::string shader_ref::collapsing_header_name() const
	{
		return editor_header_name;
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
