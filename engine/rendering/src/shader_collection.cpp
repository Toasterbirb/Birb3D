#include "Logger.hpp"
#include "ShaderCollection.hpp"
#include "ShaderSource.hpp"

#include <cassert>

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
		assert(shader_storage.empty() && "The shader collection needs to be wiped manually before it runs out of scope");
	}

	shader_ref shader_collection::register_shader(const std::string& vertex, const std::string& fragment)
	{
		assert(!vertex.empty());
		assert(!fragment.empty());

		shader_ref ref(vertex, fragment);

		assert(!shader_storage.contains(ref.hash) && "This shader has already been registered earlier");

		// Add the hashes to the shader collection
		vertex_shader_hashes[ref.vertex] = vertex;
		fragment_shader_hashes[ref.fragment] = fragment;

		FIXME("Make the shader class aware of shaders outside of the built-in shader source code collection")

		return ref;
	}

	std::shared_ptr<shader> shader_collection::get_shader(const shader_ref& ref)
	{
		assert(ref.hash != 0);

		// If the shader has been already compiled previously, return a reference to it
		if (shader_storage.contains(ref.hash))
			return shader_storage[ref.hash];

		// Check if the built-in shader names have been hashed yet
		if (vertex_shader_hashes.empty() || fragment_shader_hashes.empty())
			hash_shader_source_names();

		assert(!vertex_shader_hashes.empty());
		assert(!fragment_shader_hashes.empty());

		// Get the names of the shaders
		const std::string& vertex_name = vertex_shader_hashes.at(ref.vertex);
		const std::string& fragment_name = fragment_shader_hashes.at(ref.fragment);

		// Compile the shader and store it
		std::shared_ptr<shader> new_shader = std::make_shared<shader>(vertex_name, fragment_name);
		assert(new_shader->id != 0 && "Something went wrong with shader compiling");

		shader_storage[ref.hash] = new_shader;

		birb::log("Shader [" + vertex_name + ", " + fragment_name + "] (" + ptr_to_str(new_shader.get()) + ") stored to the shader collection");

		// Return the newly compiled shader
		return shader_storage[ref.hash];
	}

	void shader_collection::wipe()
	{
		shader_storage.clear();
	}

	void shader_collection::hash_shader_source_names()
	{
		birb::log("Hashing shader names");

		// Hash vertex shader names
		for (std::string vert : shader_src_vert_names)
		{
			u64 hash = std::hash<std::string>{}(vert);
			birb::log("vert:" + std::to_string(hash) + " -> " + vert);
			vertex_shader_hashes[hash] = vert;
		}

		// Hash fragment shader names
		for (std::string frag : shader_src_frag_names)
		{
			u64 hash = std::hash<std::string>{}(frag);
			birb::log("frag:" + std::to_string(hash) + " -> " + frag);
			fragment_shader_hashes[hash] = frag;
		}
	}
}
