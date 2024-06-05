#pragma once

#include "Types.hpp"

namespace birb
{
	template <class T>
	struct vec2;

	class rbo
	{
	public:
		explicit rbo(const vec2<i32>& dimensions);
		~rbo();
		rbo(rbo&) = delete;
		rbo(const rbo&) = delete;
		rbo(rbo&& other);

	private:
		u32 id = 0;
	};
}
