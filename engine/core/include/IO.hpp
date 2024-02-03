#pragma once

#include <string>

namespace birb
{
	namespace io
	{
		// Read from a text file
		//
		// If the file can't be opened for reading,
		// things will crash and burn
		std::string read_file(const std::string& path);

		// Write to a text file
		//
		// Returns false if the file couldn't be opened for writing
		bool write_file(const std::string& path, const std::string& text);
	};
}
