#pragma once

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
		void set_uniform_float_1(const std::string& name, float f);

	private:
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, unsigned int> uniform_locations;
	};
}
