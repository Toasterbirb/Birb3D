#include "EventBus.hpp"

#include <map>
#include <vector>

namespace birb
{
	namespace event_bus
	{
		static std::map<unsigned short, std::vector<event_obj*>> event_bus_registry;

		void register_event_id(unsigned short event_id, event_obj* obj)
		{
			event_bus_registry[event_id].push_back(obj);
		}

		void send_event(unsigned short event_id)
		{
			event_data empty_data;
			empty_data._bool.fill(false);
			send_event(event_id, empty_data);
		}

		void send_event(unsigned short event_id, const event_data& data)
		{
			// Drop events that have no subscribers
			if (!event_bus_registry.contains(event_id))
			{
				birb::log_warn("Tried to send an event with an ID (" + std::to_string(event_id) + ") that no one has registered");
				return;
			}

			std::vector<event_obj*>& event_objs = event_bus_registry.at(event_id);
			for (event_obj* obj : event_objs)
				obj->process_event(event_id, data);
		}

		void wipe()
		{
			event_bus_registry.clear();
		}
	}
}
