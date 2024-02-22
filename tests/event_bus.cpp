#include "EventBus.hpp"

#include <doctest/doctest.h>

struct test_obj : public birb::event_obj
{
	test_obj()
	{
		birb::event_bus::register_event_id(999, this); // Reserved for testing purposes
	}

	~test_obj()
	{
		birb::event_bus::unregister_event_id(999, this);
	}

	void process_event(unsigned short event_id, const birb::event_data& data)
	{
		if (event_id == 999)
			result = data._int[2];
	}

	int result = 0;
};

TEST_CASE("Event bus")
{
	test_obj obj;
	CHECK(obj.result == 0);

	birb::event_data data;
	data._int[2] = 42;
	birb::event_bus::send_event(999, data);

	CHECK(obj.result == 42);
}
