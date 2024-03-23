#pragma once

#include "Vector.hpp"

namespace birb
{
	class rbo
	{
	public:
		explicit rbo(const vec2<int>& dimensions);
		~rbo();

	private:
		unsigned int id = 0;
	};
}
