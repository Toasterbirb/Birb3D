#pragma once

namespace birb
{
	class rigidbody;

	class force
	{
	public:
		virtual void update_force(rigidbody& rigidbody) = 0;
	};
}
