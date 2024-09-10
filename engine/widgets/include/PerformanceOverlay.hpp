#pragma once

#include "WidgetBase.hpp"

#include <array>

#ifdef BIRB_PLATFORM_LINUX
#include <sys/resource.h>
#endif

namespace birb
{
	class timestep;

	namespace overlay
	{
		class performance : public widget_base
		{
		public:
			explicit performance(const timestep& ts);
			performance(timestep& ts, const char* collapsing_menu);
			~performance();
			performance(performance&) = delete;
			performance(const performance&) = delete;
			performance(performance&&) = default;

			void draw() override;

		private:
			const char* collapsing_menu_name = "";

			// Reference to the timestep used in the gameloop
			// This is used to get frametime data
			const timestep& ts;

			std::array<char, 12> overlay_text_buffer;


			// -----------------------------------------------------------------
			// --- Linux specific stuff, should be ignored in Windows builds ---
			// -----------------------------------------------------------------

			// !! NOTE !!
			// This way of blocking out variables in the header file has caused crashes
			// in the past in weird ways. This is left here on purpose to see if that problem
			// is not a thing anymore in gcc14 onwards.
			//
			// If this causes a crash, there's a chance that there's some bug in the compiler
			// that needs fixing
#ifdef BIRB_PLATFORM_LINUX
			i32 pid; // Our process ID

			// Fill the memory history array with zeroes and get the PID
			void setup_memory_history();

			// Get resident memory usage in megabytes
			i64 resident_memory_usage() const;

			std::array<f32, 16> memory_history;
#endif
			// -----------------------------------------------------------------
		};
	}
}
