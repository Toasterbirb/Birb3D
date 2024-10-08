#pragma once

#include "Assert.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <algorithm>
#include <concepts>
#include <mutex>
#include <random>

namespace birb
{
	class random
	{
	public:
		/**
		 * @brief Construct a random number generator with a random seed
		 *
		 * The seed is created from the current time and an increasing
		 * counter. The counter is used to make sure that subsequent seeds are not identical
		 * if the constructor is called repeatedly
		 */
		random();

		~random() = default;

		// Copying the random engine might cause trouble, since
		// the state of it and the seed would also get copied
		random(random&) = delete;
		random(const random&) = delete;

		// Moving is okay though
		random(random&&) = default;

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
		T range(const T min, const T max)
		{
			static_assert(!std::floating_point<T>, "Random integers can't be generated with a floating point range");
			ensure(min <= max);

			const T value = rng_engine() % (max + 1 - min) + min;
			ensure(value >= min);
			ensure(value <= max);
			return value;
		}

		/**
		 * @brief Generate a random floating point value between min and max
		 *
		 * @warning Using this method with integer values will return non-random values. Only use this method with floating point ranges
		 */
		template<typename T>
		T range_float(const T min, const T max)
		{
			static_assert(std::floating_point<T>, "range_float() only works with floating point ranges");
			ensure(min <= max);

			const T multiplier = (static_cast<T>(rng_engine())) / rng_engine.max();
			const T value = (multiplier * (max - min)) + min;
			ensure(value >= min);
			ensure(value <= max);
			return value;
		}

		vec2<i32> range_vec2_int(const i32 min, const i32 max)
		{
			return vec2<i32>(range(min, max), range(min, max));
		}

		template<typename T>
		vec2<T> range_vec2_float(const T min, const T max)
		{
			return vec2<T>(range_float(min, max), range_float(min, max));
		}

		vec3<i32> range_vec3_int(const i32 min, const i32 max)
		{
			return vec3<i32>(range(min, max), range(min, max), range(min, max));
		}

		template<typename T>
		vec3<T> range_vec3_float(const T min, const T max)
		{
			return vec3<T>(range_float(min, max), range_float(min, max), range_float(min, max));
		}

		template<typename T>
		std::vector<T> shuffle(std::vector<T> vec)
		{
			std::shuffle(vec.begin(), vec.end(), rng_engine);
			return vec;
		}

		template<typename T>
		void shuffle_in_place(std::vector<T>& vec)
		{
			std::shuffle(vec.begin(), vec.end(), rng_engine);
		}

		template<typename T, size_t N>
		std::array<T, N> shuffle(std::array<T, N> arr)
		{
			std::shuffle(arr.begin(), arr.end(), rng_engine);
			return arr;
		}

		template<typename T, size_t N>
		void shuffle_in_place(std::array<T, N>& arr)
		{
			std::shuffle(arr.begin(), arr.end(), rng_engine);
		}

	private:
		std::mt19937_64 rng_engine;

		// This counter is meant to help with repeated calls to creating
		// new instances of the random class
		static inline u32 counter = 0;
		static inline std::mutex counter_mutex;
	};
}
