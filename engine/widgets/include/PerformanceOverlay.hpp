#pragma once

#include "Timestep.hpp"
#include "WidgetBase.hpp"

#include <array>
#include <string>
#include <sys/resource.h>

namespace birb
{
	namespace overlay
	{
		class performance : public widget_base
		{
		public:
			explicit performance(timestep& ts);
			performance(timestep& ts, const char* collapsing_menu);
			~performance();
			void draw() override;

		private:
			const char* collapsing_menu_name = "";
			bool is_overlay = true;

#ifdef BIRB_PLATFORM_LINUX
			int pid; // Our process ID

			// Fill the memory history array with zeroes and get the PID
			void setup_memory_history();

			// Get resident memory usage in megabytes
			long resident_memory_usage() const;

			std::array<float, 16> memory_history;
#endif

			// Reference to the timestep used in the gameloop
			// This is used to get frametime data
			timestep& ts;

			std::array<char, 12> overlay_text_buffer;
		};
	}
}
