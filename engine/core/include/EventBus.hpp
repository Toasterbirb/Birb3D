#pragma once

#include "Events.hpp"

#include <array>
#include <cstddef>

namespace birb
{
	constexpr std::size_t event_data_size = 24; // Size of the event_data union in bytes

	union event_data
	{
		std::array<i8,		event_data_size / sizeof(i8)>	_i8 = {};
		std::array<i16,		event_data_size / sizeof(i16)>	_i16;
		std::array<i32,		event_data_size / sizeof(i32)>	_i32;
		std::array<i64,		event_data_size / sizeof(i64)>	_i64;
		std::array<f32,		event_data_size / sizeof(f32)>	_f32;
		std::array<f64,		event_data_size / sizeof(f64)>	_f64;
		std::array<bool,	event_data_size / sizeof(bool)>	_bool;
		std::array<char,	event_data_size / sizeof(char)>	_char;
	};

	// Interface that all objects that deal with events should inherit from
	struct event_obj
	{
		virtual void process_event(u16 event_id, const event_data& data) = 0;
	};

	/**
	 * @brief System for sending events around the program
	 */
	namespace event_bus
	{
		/**
		 * @brief Register a new event
		 *
		 * @warning For every event ID you register, you should also unregister them
		 * in the destructor. Otherwise you might be walking on thin ice and the program
		 * can crash if the event ID gets called after the object goes out of scope
		 *
		 * @param event_id Identifier for the event
		 * @param obj Pointer to the object that will receive the event
		 */
		void register_event_id(u16 event_id, event_obj* obj);

		/**
		 * @brief Unregister an object from an event ID
		 *
		 * @param event_id Identifier for the event
		 * @param obj Pointer to the object that will be removed from the event ID registry
		 */
		void unregister_event_id(u16 event_id, event_obj* obj);

		/**
		 * @brief Send an event to all event objects that have subscribed to the given ID
		 */
		void send_event(u16 event_id);

		/**
		 * @brief Send an event and data to all event objects that have subscribed to the given ID
		 */
		void send_event(u16 event_id, const event_data& data);

		/**
		 * @brief Clear the registry of all event ids and object pointers
		 */
		void wipe();
	}
}
