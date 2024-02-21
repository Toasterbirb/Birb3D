#pragma once

#include <array>
namespace birb
{
	constexpr size_t event_data_size = 24; // Size of the event_data union in bytes

	union event_data
	{
		std::array<bool, event_data_size / sizeof(bool)> _bool;
		std::array<char, event_data_size / sizeof(char)> _char;
		std::array<int, event_data_size / sizeof(int)> _int;
		std::array<float, event_data_size / sizeof(float)> _float;
		std::array<double, event_data_size / sizeof(double)> _double;
		std::array<long, event_data_size / sizeof(long)> _long;
	};

	// Interface that all objects that deal with events should inherit from
	struct event_obj
	{
		virtual void process_event(unsigned short event_id, const event_data& data) = 0;
	};

	namespace event_bus
	{
		/**
		 * @brief Register a new event
		 *
		 * @param event_id Identifier for the event
		 * @param obj Pointer to the object that will receive the event
		 */
		void register_event_id(unsigned short event_id, event_obj* obj);

		/**
		 * @brief Send an event to all event objects that have subscribed to the given ID
		 */
		void send_event(unsigned short event_id);

		/**
		 * @brief Send an event and data to all event objects that have subscribed to the given ID
		 */
		void send_event(unsigned short event_id, const event_data& data);

		/**
		 * @brief Clear the registry of all event ids and object pointers
		 */
		void wipe();
	}
}
