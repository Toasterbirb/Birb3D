#pragma once

#include "Types.hpp"

#include <string>

namespace birb
{
	class stopwatch
	{
	public:
		explicit stopwatch(const std::string& name = "Stopwatch");
		~stopwatch();
		stopwatch(const stopwatch&) = delete;
		stopwatch(stopwatch&) = delete;
		stopwatch(stopwatch&&) = delete;

		void reset();
		f64 stop(const bool quiet = false);


		/**
		 * @brief Format time in seconds down to the most fitting unit
		 *
		 * Supports seconds, milliseconds, microseconds and nanoseconds
		 */
		static std::string format_time(const f64 seconds);

	private:
		static inline const std::string text_color_code = "\033[1;35m";
		const std::string name;
		f64 start = 0.0;
		bool is_stopped = false;
	};
}
