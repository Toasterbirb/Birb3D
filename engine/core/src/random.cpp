#include "Random.hpp"

#include <ctime>

#ifdef BIRB_PLATFORM_LINUX
#include "Logger.hpp"
#include <fstream>
#endif

namespace birb
{
	random::random()
	{
		unsigned int seed = time(0);

#ifdef BIRB_PLATFORM_LINUX
		// Get rng by reading a few bytes from /dev/urandom
		std::ifstream urandom("/dev/urandom", std::ios::binary);
		if (urandom.is_open())
			urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
		else
			birb::log_warn("Unable to open /dev/urandom for reading! Falling back to using time as the seed");
#endif

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
