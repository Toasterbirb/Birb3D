#pragma once

#include "Timestep.hpp"

#include <array>
#include <sys/resource.h>

namespace birb
{
	namespace widget
	{
		class performance
		{
		public:
			performance(timestep& ts);
			~performance();
			void draw();

		private:
			int pid; // Our process ID

			// Get resident memory usage in megabytes
			long resident_memory_usage() const;

			std::array<float, 16> memory_history;

			// Reference to the timestep used in the gameloop
			// This is used to get frametime data
			timestep& ts;

			std::array<char, 12> overlay_text_buffer;
		};
	}
}
