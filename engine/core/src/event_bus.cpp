#include "EventBus.hpp"
#include "Logger.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

namespace birb
{
	namespace event_bus
	{
		static std::unordered_map<unsigned short, std::vector<event_obj*>> event_bus_registry;

		void register_event_id(unsigned short event_id, event_obj* obj)
		{
			assert(obj != nullptr && "Can register event_id for a null pointer");

			birb::log("Event ID " + std::to_string(event_id) + " registered to " + birb::ptr_to_str(obj));

			event_bus_registry[event_id].push_back(obj);
		}

		void unregister_event_id(unsigned short event_id, event_obj* obj)
		{
			assert(obj != nullptr && "Can unregister event_id for a null pointer");

			birb::log("Event ID " + std::to_string(event_id) + " unregistered for " + birb::ptr_to_str(obj));

			std::vector<event_obj*>& event_objs = event_bus_registry.at(event_id);
			event_objs.erase(std::remove_if(event_objs.begin(), event_objs.end(),
				[&obj](const event_obj* ptr)
				{
					return ptr && (ptr == obj);
				}));
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

			const std::vector<event_obj*>& event_objs = event_bus_registry.at(event_id);
			for (event_obj* obj : event_objs)
				obj->process_event(event_id, data);
		}

		void wipe()
		{
			birb::log("Event bus wiped! All event_id registrations have been cleared");
			event_bus_registry.clear();
		}
	}
}
