#pragma once

#include "Random.hpp"

namespace birb
{
	class uuid
	{
	public:
		uuid() = delete;
		~uuid() = delete;
		uuid(const uuid&) = delete;
		uuid(uuid&) = delete;
		uuid(uuid&&) = delete;

		/**
		 * @brief Returns a random 64bit unsigned integer
		 *
		 * The uniqueness of this identifier is not guaranteed, but the
		 * chance of collision *should* be vanishingly small
		 */
		static u64 generate();

	private:
		static inline random rng;
	};
}
