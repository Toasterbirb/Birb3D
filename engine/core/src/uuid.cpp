#include "UUID.hpp"

namespace birb
{
	u64 uuid::generate()
	{
		return rng.next();
	}
}
