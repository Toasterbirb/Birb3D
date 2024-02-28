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

		bool has_uniform_var(const std::string& name) const;
		void add_uniform_location(const std::string& name);
		void add_uniform_location(const std::vector<std::string>& names);

		void set_mat4(const std::string& name, const glm::mat4 mat4);
		void set_vec3(const std::string& name, const glm::vec3 vector);
		void set_float(const std::string& name, const float f);
		void set_int(const std::string& name, const int i);

	private:
		void compile_shader(const std::string& vertex, const std::string& fragment);
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, unsigned int> uniform_locations;
	};
}
