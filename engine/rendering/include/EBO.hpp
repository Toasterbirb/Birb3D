#pragma once

#include <vector>

namespace birb
{
	// Element array buffer
	class ebo
	{
	public:
		explicit ebo(std::vector<unsigned int> indices);

		// Reference to the element array buffer
		unsigned int id;

		void bind();
		void unbind();
		void unload();

	private:

	};
}
