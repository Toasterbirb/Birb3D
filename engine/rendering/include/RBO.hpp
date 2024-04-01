#pragma once

#include "Vector.hpp"

namespace birb
{
	class rbo
	{
	public:
		explicit rbo(const vec2<i32>& dimensions);
		rbo(rbo&) = delete;
		rbo(const rbo&) = delete;
		~rbo();

	private:
		u32 id = 0;
	};
}
