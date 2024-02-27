#pragma once

#include <cassert>
#include <glad/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace birb
{
	class shader
	{
	public:
		shader(const std::string& shader_name);
		shader(const std::string& vertex, const std::string& fragment);
		~shader();

		// Reference to the shader program
		unsigned int id;

		// Activate the shader program
		void activate();

		void add_uniform_location(const std::string& name);
		void add_uniform_location(const std::vector<std::string>& names);
		void set_var_mat4(const std::string& name, glm::mat4 mat4);
		void set_var_vec3f(const std::string& name, vec3<float> vector);

		template<typename T>
		void set_var(const std::string& name, T value)
		{
			activate();
			assert(uniform_locations.contains(name) && "Tried to access a uniform variable that wasn't added");
			glUniform1f(uniform_locations[name], value);
		}

	private:
		void compile_shader(const std::string& vertex, const std::string& fragment);
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, unsigned int> uniform_locations;
	};
}
