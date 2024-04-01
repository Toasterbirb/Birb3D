#pragma once

#include "Types.hpp"
#include "Vector.hpp"

#include <random>
#include <mutex>
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
		explicit random(u32 seed);

		/**
		 * @brief Change the seed of the random number engine
		 *
		 * @param seed
		 */
		void seed(u32 seed);

		/**
		 * @brief Get the next random number from the random number engine
		 */
		u64 next();

		/**
		 * @brief Generate a random integer value between min and max (inclusive)
		 */
		template<typename T>
		T range(T min, T max)
		{
			static_assert(!std::is_same<T, float>::value && !std::is_same<T, double>::value, "Random integers can't be generated with a floating point range");
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
			static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "range_float() only works with floating point ranges");
			assert(min <= max);

			T multiplier = (static_cast<T>(rng_engine())) / rng_engine.max();
			T value = (multiplier * (max - min)) + min;
			assert(value >= min);
			assert(value <= max);
			return value;
		}

		vec2<i32> range_vec2_int(i32 min, i32 max)
		{
			return vec2<i32>(range(min, max), range(min, max));
		}

		template<typename T>
		vec2<T> range_vec2_float(T min, T max)
		{
			return vec2<T>(range_float(min, max), range_float(min, max));
		}

		vec3<i32> range_vec3_int(i32 min, i32 max)
		{
			return vec3<i32>(range(min, max), range(min, max), range(min, max));
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
		static inline u32 counter = 0;
		static inline std::mutex counter_mutex;
	};
}
