#pragma once

#include <sys/resource.h>

namespace birb
{
	class performance_widget
	{
	public:
		performance_widget();
		float frametime = 0;
		void draw();

	private:
		int pid; // Our process ID

		int fps() const;

		// Get resident memory usage in megabytes
		long resident_memory_usage() const;
	};
}
