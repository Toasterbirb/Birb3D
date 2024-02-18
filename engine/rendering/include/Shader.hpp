#pragma once

#include <string>

namespace birb
{
	class shader
	{
	public:
		shader(const std::string& vertex, const std::string& fragment);

		// Reference to the shader program
		unsigned int id;

		// Activate the shader program
		void activate();

		// Delete the shader program
		void unload();

	private:
		void compile_errors(unsigned int shader, const std::string& type);
	};
}
