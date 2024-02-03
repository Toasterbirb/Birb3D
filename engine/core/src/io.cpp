#include "IO.hpp"
#include "Logger.hpp"

#include <cassert>
#include <fstream>
#include <string>

namespace birb
{
	namespace io
	{
		std::string read_file(const std::string& path)
		{
			assert(!path.empty() && "Can't read from an empty filepath");

			std::ifstream file;
			file.open(path, std::ios::in);
			if (!file.is_open())
				birb::log_fatal("Can't open a file at " + path);

			std::string file_contents;
			while (file.good())
				file_contents.push_back(file.get());

			// Fix the null terminator
			file_contents.erase(file_contents.size() - 1, 1);

			return file_contents;
		}

		bool write_file(const std::string& path, const std::string& text)
		{
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
	}
}
