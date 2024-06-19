#include "Assert.hpp"
#include "Crypto.hpp"
#include "IO.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Random.hpp"

#include <fstream>
#include <future>
#include <nlohmann/json.hpp>
#include <stb_image.h>
#include <string>

namespace birb
{
	namespace io
	{
		static const std::string obfuscation_magic_bytes = "HIDDEN";
		static random json_obfuscation_rng;

		std::string read_file(const std::string& path)
		{
			PROFILER_SCOPE_IO_FN();
			ensure(!path.empty(), "Can't read from an empty filepath");

			std::ifstream file;
			file.open(path, std::ios::in);
			if (!file.is_open())
				birb::log_fatal(2, "Can't open a file at " + path);

			std::string file_contents;

			// Figure out the file size and resize the result string to fit it
			file.seekg(0, std::ios::end);
			file_contents.resize(file.tellg());

			// Go back to the start of the file
			file.seekg(0, std::ios::beg);

			// Read everything at once
			file.read(&file_contents[0], file_contents.size());

			// Check if the data needs to be decrypted
			if (!file_contents.starts_with(obfuscation_magic_bytes))
				return file_contents;

			// Remove the magic bytes
			file_contents.erase(0, obfuscation_magic_bytes.size());

			return crypto::decrypt(file_contents);
		}

		std::future<std::string> read_file_async(const std::string &path)
		{
			return std::async(std::launch::async, read_file, path);
		}

		bool write_file(const std::string& path, const std::string& text, const bool obfuscate)
		{
			PROFILER_SCOPE_IO_FN();
			ensure(!path.empty(), "Can't write to an empty filepath");

			std::ofstream file;
			file.open(path);
			if (!file.is_open())
			{
				birb::log_error("Couldn't write to file path " + path);
				return false;
			}

			if (obfuscate)
				file << obfuscation_magic_bytes + crypto::encrypt(text);
			else
				file << text;

			return true;
		}

		std::future<bool> write_file_async(const std::string &path, const std::string& text, const bool obfuscate)
		{
			return std::async(std::launch::async, write_file, path, text, obfuscate);
		}

		bool write_json_file(const std::string& path, const nlohmann::json& json, const bool obfuscate)
		{
			PROFILER_SCOPE_IO_FN();
			ensure(!path.empty(), "Can't write to an empty filepath");

			std::ofstream file;
			file.open(path);
			if (!file.is_open())
			{
				birb::log_error("Couldn't write to file path " + path);
				return false;
			}

			if (obfuscate)
			{
				// Padding is added to the start and end of the string before
				// obfuscation to change the length of the string and thus
				// make the outcome of the obfuscation different every time without
				// affecting the parsing of the json data
				constexpr i32 max_padding_amount = 16;
				const std::string start_padding = std::string(json_obfuscation_rng.range(0, max_padding_amount), ' ');
				const std::string end_padding = std::string(json_obfuscation_rng.range(0, max_padding_amount), ' ');

				std::stringstream ss;

				ss << json << std::endl;
				file << obfuscation_magic_bytes + crypto::encrypt(start_padding + ss.str() + end_padding);
			}
			else
			{
				file << json << std::endl;
			}

			return true;
		}

		bool write_bson_file(const std::string& path, const nlohmann::json& json)
		{
			PROFILER_SCOPE_IO_FN();
			ensure(!path.empty(), "Can't write to an empty filepath");

			std::ofstream file;
			file.open(path);
			if (!file.is_open())
			{
				birb::log_error("Couldn't write to file path " + path);
				return false;
			}

			std::vector<u8> bson = nlohmann::json::to_bson(json);

			for (u8 byte : bson)
				file << byte << std::endl;

			return true;
		}
	}
}
