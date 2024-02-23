#pragma once

#include <cassert>
#include <glad/gl.h>
#include <string>
#include <unordered_map>

namespace birb
{
	class shader
	{
	public:
		shader(const std::string& vertex, const std::string& fragment);
		~shader();

		// Reference to the shader program
		unsigned int id;

		// Activate the shader program
		void activate();

		void add_uniform_location(const std::string& name);

		template<typename T>
		void set_var(const std::string& name, T f)
		{
			activate();
			assert(uniform_locations.contains(name) && "Tried to access a uniform variable that wasn't added");
			glUniform1f(uniform_locations[name], f);
		}

	private:
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, unsigned int> uniform_locations;
	};
}
