#include "IO.hpp"
#include "Logger.hpp"

#include <cassert>
#include <fstream>
#include <future>
#include <string>
#include <microprofile.h>

#define PROFILER_GROUP "I/O"
#define PROFILER_COLOR MP_BLUE

namespace birb
{
	namespace io
	{
		std::string read_file(const std::string& path)
		{
			MICROPROFILE_SCOPEI(PROFILER_GROUP, "Read file", PROFILER_COLOR);
			assert(!path.empty() && "Can't read from an empty filepath");

			std::ifstream file;
			file.open(path, std::ios::in);
			if (!file.is_open())
				birb::log_fatal("Can't open a file at " + path);

			std::string file_contents;

			// Figure out the file size and resize the result string to fit it
			file.seekg(0, std::ios::end);
			file_contents.resize(file.tellg());

			// Go back to the start of the file
			file.seekg(0, std::ios::beg);

			// Read everything at once
			file.read(&file_contents[0], file_contents.size());

			return file_contents;
		}

		std::future<std::string> read_file_async(const std::string &path)
		{
			return std::async(std::launch::async, read_file, path);
		}

		bool write_file(const std::string& path, const std::string& text)
		{
			MICROPROFILE_SCOPEI(PROFILER_GROUP, "Write file", PROFILER_COLOR);
			assert(!path.empty() && "Can't write to an empty filepath");

			std::ofstream file;
			file.open(path);
			if (!file.is_open())
			{
				birb::log_error("Couldn't write to file path " + path);
				return false;
			}

			file << text;
			file.close();
			return true;
		}

		std::future<bool> write_file_async(const std::string &path, const std::string& text)
		{
			return std::async(std::launch::async, write_file, path, text);
		}
	}
}
