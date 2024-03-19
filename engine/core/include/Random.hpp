#pragma once

#include "Vector.hpp"

#include <random>
#include <type_traits>

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
			static_assert(!std::is_same<T, float>::value && "Random integers can't be generated with a floating point range");
			static_assert(!std::is_same<T, double>::value && "Random integers can't be generated with a floating point range");
			assert(min <= max);

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
#define RANGE_FLOAT_STATIC_ASSERT_MSG "Calling range_float() with an integer range will always yield the min value as the result"
			static_assert(!std::is_same<T, short>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, unsigned short>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, int>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, unsigned int>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, long>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, unsigned long>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, long long>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
			static_assert(!std::is_same<T, unsigned long long>::value && RANGE_FLOAT_STATIC_ASSERT_MSG);
#undef RANGE_FLOAT_STATIC_ASSERT_MSG

			assert(min <= max);

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

		// This counter is meant to help with repeated calls to creating
		// new instances of the random class
		//
		// TODO: Make this thread safe
		static inline unsigned int counter = 0;
	};
}
