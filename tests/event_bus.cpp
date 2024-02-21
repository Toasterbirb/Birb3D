#include "EventBus.hpp"

#include <doctest/doctest.h>

struct test_obj : public birb::event_obj
{
	test_obj()
	{
		birb::event_bus::register_event_id(999, this); // Reserved for testing purposes
	}

	void process_event(unsigned short event_id, const birb::event_data& data)
	{
		if (event_id == 999)
			success = data._int[2];
	}

	bool success = false;
};

TEST_CASE("Event bus")
{
	test_obj obj;
	CHECK(obj.success == false);

	birb::event_data data;
	data._bool[2] = 42;
	birb::event_bus::send_event(999, data);

	CHECK(obj.success == true);
}
