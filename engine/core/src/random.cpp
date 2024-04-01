#include "Random.hpp"

#include <ctime>

namespace birb
{
	random::random()
	{
		u32 seed = time(0);;

		counter_mutex.lock();
		seed += counter++;
		counter_mutex.unlock();

		rng_engine.seed(seed);
	}

	random::random(u32 seed)
	{
		rng_engine.seed(seed);
	}

	void random::seed(u32 seed)
	{
		rng_engine.seed(seed);
	}

	u64 random::next()
	{
		return rng_engine();
	}
}
