#include "Logger.hpp"
#include "ShaderCollection.hpp"

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

	std::shared_ptr<shader> shader_collection::get_shader(const shader_ref& ref)
	{
		return get_shader(*ref.vertex, *ref.fragment);
	}

	std::shared_ptr<shader> shader_collection::get_shader(const std::string& vertex, const std::string& fragment)
	{
		assert(!vertex.empty());
		assert(!fragment.empty());

		assert(vertex.find(';') == std::string::npos && "The vertex shader name cannot contain the char ';'");
		assert(fragment.find(';') == std::string::npos && "The fragment shader name cannot contain the char ';'");

		const std::string key = vertex + ";" + fragment;

		// If the shader has been already compiled previously, return a reference to it
		if (shader_storage.contains(key))
			return shader_storage[key];

		// Compile the shader and store it
		std::shared_ptr<shader> new_shader = std::make_shared<shader>(vertex, fragment);
		assert(new_shader->id != 0 && "Something went wrong with shader compiling");

		shader_storage[key] = new_shader;

		birb::log("Shader [" + vertex + ", " + fragment + "] (" + ptr_to_str(new_shader.get()) + ") stored to the shader collection");

		// Return the newly compiled shader
		return shader_storage[key];
	}

	void shader_collection::wipe()
	{
		shader_storage.clear();
	}
}
