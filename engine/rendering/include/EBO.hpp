#pragma once

#include <vector>

namespace birb
{
	// Element array buffer
	class ebo
	{
	public:
		explicit ebo(const std::vector<unsigned int>& indices);
		~ebo();

		// Reference to the element array buffer
		unsigned int id;

		void bind();
		void unbind();

	private:

	};
}
