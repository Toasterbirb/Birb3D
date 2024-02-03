#pragma once

#include <string>

namespace birb
{
	class shader
	{
	public:
		shader(const std::string& vertex_path, const std::string& fragment_path);

		void activate();
		void unload();

	private:

	};
}
