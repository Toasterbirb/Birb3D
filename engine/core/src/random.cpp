#include "Random.hpp"

#include <ctime>

namespace birb
{
	random::random()
	{
		unsigned int seed = time(0);;

		counter_mutex.lock();
		seed += counter++;
		counter_mutex.unlock();

		rng_engine.seed(seed);
	}

	random::random(unsigned int seed)
	{
		rng_engine.seed(seed);
	}

	void random::seed(unsigned int seed)
	{
		rng_engine.seed(seed);
	}

	unsigned long random::next()
	{
		return rng_engine();
	}
}
