#pragma once

#include "Timestep.hpp"
#include "WidgetBase.hpp"

#include <array>
#include <string>

#ifdef BIRB_PLATFORM_LINUX
#include <sys/resource.h>
#endif

namespace birb
{
	namespace overlay
	{
		class performance : public widget_base
		{
		public:
			explicit performance(const timestep& ts);
			performance(timestep& ts, const char* collapsing_menu);
			~performance();
			void draw() override;

		private:
			const char* collapsing_menu_name = "";

			// Reference to the timestep used in the gameloop
			// This is used to get frametime data
			const timestep& ts;

			std::array<char, 12> overlay_text_buffer;

#ifdef BIRB_PLATFORM_LINUX
			i32 pid; // Our process ID

			// Fill the memory history array with zeroes and get the PID
			void setup_memory_history();

			// Get resident memory usage in megabytes
			i64 resident_memory_usage() const;

			std::array<f32, 16> memory_history;
#endif
		};
	}
}
