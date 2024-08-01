#include "Assert.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderRef.hpp"
#include "ShaderSource.hpp"

#include <array>

namespace birb
{
	shader_collection::~shader_collection()
	{
		// The static shader_storage hashmap outlives the window and thus
		// causes a confusing segmentation fault due to scope issues
		// if the shader_storage isn't manually cleared
		//
		// The window should call the wipe() function before destroying itself
		// but this assert is here just in case
		ensure(shader_storage.empty(), "The shader collection needs to be wiped manually before it runs out of scope");
	}

	void shader_collection::precompile_basic_shaders()
	{
		// Make sure, that the shader source names have been hashed
		hash_shader_source_names();

		birb::log("Precompiling shaders...");

		const std::array<shader_ref, 6> precompiled_shaders = {
			shader_ref("color", "color"),
			shader_ref("text", "text"),
			shader_ref("texture", "texture"),
			shader_ref("post_process", "post_process"),
			shader_ref("default", "default"),
			shader_ref("vertex_color", "vertex_color"),
		};

		for (const shader_ref& ref : precompiled_shaders)
			compile_shader(ref);

		birb::log("Shader precompiling finished!");
	}

	shader_ref shader_collection::register_shader(const std::string& vertex, const std::string& fragment)
	{
		hash_shader_source_names();

		ensure(!vertex.empty());
		ensure(!fragment.empty());

		shader_ref ref(vertex, fragment);

		ensure(!shader_storage.contains(ref.hash()), "This shader has already been registered earlier");

		// Add the hashes to the shader collection
		vertex_shader_hashes[ref.vertex()] = vertex;
		fragment_shader_hashes[ref.fragment()] = fragment;

		return ref;
	}

	bool shader_collection::is_shader_registered(const shader_ref& shader)
	{
		return vertex_shader_hashes.contains(shader.vertex())
			&& fragment_shader_hashes.contains(shader.fragment());
	}

	std::shared_ptr<shader> shader_collection::get_shader(const shader_ref& ref)
	{
		ensure(ref.hash() != 0);

		// If the shader has been already compiled previously, return a reference to it
		if (shader_storage.contains(ref.hash()))
			return shader_storage[ref.hash()];

		// Check if the built-in shader names have been hashed yet
		if (vertex_shader_hashes.empty() || fragment_shader_hashes.empty())
			hash_shader_source_names();

		ensure(!vertex_shader_hashes.empty());
		ensure(!fragment_shader_hashes.empty());

		// Compile the shader and return a reference to it
		return compile_shader(ref);
	}

	void shader_collection::wipe()
	{
		shader_storage.clear();
	}

	std::shared_ptr<shader> shader_collection::compile_shader(const shader_ref& ref)
	{
		ensure(!vertex_shader_hashes.empty(), "Vertex names need to be hashed before shaders can be compiled");
		ensure(!fragment_shader_hashes.empty(), "Vertex names need to be hashed before shaders can be compiled");
		ensure(!shader_storage.contains(ref.hash()), "Tried to compile and store a shader to the collection that has already been compiled previously");

		// Get the names of the shaders
		ensure(vertex_shader_hashes.contains(ref.vertex()), "Tried to compile a non-existent vertex shader. Maybe you forgot to register it?");
		ensure(fragment_shader_hashes.contains(ref.fragment()), "Tried to compile a non-existent fragment shader. Maybe you forgot to register it?");
		const std::string& vertex_name = vertex_shader_hashes.at(ref.vertex());
		const std::string& fragment_name = fragment_shader_hashes.at(ref.fragment());

		// Compile the shader and store it
		std::shared_ptr<shader> new_shader = std::make_shared<shader>(vertex_name, fragment_name);
		ensure(new_shader->id != 0, "Something went wrong with shader compiling");
		shader_storage[ref.hash()] = new_shader;

		return new_shader;
	}

	void shader_collection::hash_shader_source_names()
	{
		// Skip shader source name hashing if they have already
		// been hashed earlier
		if (builtin_shaders_hashed)
			return;

		birb::log("Hashing shader names");

		// Hash vertex shader names
		for (std::string vert : shader_src_vert_names)
		{
			u64 hash = std::hash<std::string>{}(vert);
			birb::log("vert: ", hash, " -> ", vert);
			vertex_shader_hashes[hash] = vert;
		}

		// Hash fragment shader names
		for (std::string frag : shader_src_frag_names)
		{
			u64 hash = std::hash<std::string>{}(frag);
			birb::log("frag: ", hash, " -> ", frag);
			fragment_shader_hashes[hash] = frag;
		}

		builtin_shaders_hashed = true;
	}
}
