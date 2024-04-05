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
		void stop();

	private:
		const std::string name;
		double start = 0.0;
		bool is_stopped = false;
	};
}
