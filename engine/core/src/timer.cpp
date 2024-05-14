#include "Assert.hpp"
#include "Timer.hpp"

namespace birb
{
	timer::timer(const f32 duration)
	:time_left(duration), duration(duration)
	{
		ensure(duration > 0);
	}

	void timer::tick(const f32 deltatime)
	{
		time_left -= deltatime;
	}

	void timer::reset()
	{
		time_left = duration;
	}

	void timer::end()
	{
		time_left = 0;
	}

	bool timer::done()
	{
		return time_left <= 0;
	}
}
