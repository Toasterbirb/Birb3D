#pragma once

#include "Vector.hpp"

#include <random>

namespace birb
{
	class random
	{
	public:
		/**
		 * @brief Construct a random number generator with a random seed
		 *
		 * On linux this will get the seed by reading a few bytes from
		 * /dev/urandom. If this file cannot be opened (or we are not
		 * on linux) time will be used as the seed instead.
		 */
		random();

		/**
		 * @brief Construct a random number generator with a set seed
		 */
		explicit random(unsigned int seed);

		/**
		 * @brief Change the seed of the random number engine
		 *
		 * @param seed
		 */
		void seed(unsigned int seed);

		/**
		 * @brief Get the next random number from the random number engine
		 */
		unsigned long next();

		/**
		 * @brief Generate a random integer value between min and max (inclusive)
		 */
		template<typename T>
		T range(T min, T max)
		{
			T value = rng_engine() % (max + 1 - min) + min;
			assert(value >= min);
			assert(value <= max);
			return value;
		}

		/**
		 * @brief Generate a random floating point value between min and max
		 *
		 * @warning Using this method with integer values will return non-random values. Only use this method with floating point ranges
		 */
		template<typename T>
		T range_float(T min, T max)
		{
			T multiplier = (static_cast<T>(rng_engine())) / rng_engine.max();
			T value = (multiplier * (max - min)) + min;
			assert(value >= min);
			assert(value <= max);
			return value;
		}

		vec2<int> range_vec2_int(int min, int max)
		{
			return vec2<int>(range(min, max), range(min, max));
		}

		template<typename T>
		vec2<T> range_vec2_float(T min, T max)
		{
			return vec2<T>(range_float(min, max), range_float(min, max));
		}

		vec3<int> range_vec3_int(int min, int max)
		{
			return vec3<int>(range(min, max), range(min, max), range(min, max));
		}

		template<typename T>
		vec3<T> range_vec3_float(T min, T max)
		{
			return vec3<T>(range_float(min, max), range_float(min, max), range_float(min, max));
		}

	private:
		std::mt19937_64 rng_engine;
	};
}
