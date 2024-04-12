#pragma once

namespace birb
{
	class timer
	{
	public:
		explicit timer(const std::string& name = "Timer");
		~timer();
		timer(const timer&) = default;
		timer(timer&) = default;

		void reset();
		double stop(const bool quiet = false);


		/**
		 * @brief Format time in seconds down to the most fitting unit
		 *
		 * Supports seconds, milliseconds, microseconds and nanoseconds
		 */
		static std::string format_time(const double seconds);

	private:
		const std::string name;
		double start = 0.0;
		bool is_stopped = false;
	};
}
