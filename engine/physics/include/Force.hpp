#pragma once

#include "Rigidbody.hpp"

namespace birb
{
	class force
	{
	public:
		virtual void update_force(rigidbody& rigidbody) = 0;
	};
}
