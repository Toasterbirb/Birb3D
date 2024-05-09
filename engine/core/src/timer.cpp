#include "Timer.hpp"

namespace birb
{
	timer::timer(const f32 duration)
	:time_left(duration), duration(duration)
	{}

	void timer::tick(const f32 deltatime)
	{
		time_left -= deltatime;
	}

	void timer::reset()
	{
		time_left = duration;
	}

	bool timer::done()
	{
		return time_left <= 0;
	}
}
