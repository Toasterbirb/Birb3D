#pragma once

#include "Types.hpp"

namespace birb
{
	class timer
	{
	public:
		explicit timer(const f32 duration);
		~timer() = default;
		timer(const timer&) = default;
		timer(timer&) = default;

		void tick(const f32 deltatime);
		void reset();

		/**
		 * @brief Skip straight to the end of the timer
		 */
		void end();

		/**
		 * @return True if time remaining is <= 0
		 */
		bool done();

		operator float()
		{
			return time_left;
		}

	private:
		float time_left = 0;
		const float duration = 0;
	};
}
