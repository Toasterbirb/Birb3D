#pragma once

#include <future>
#include <nlohmann/json_fwd.hpp>
#include <string>

/**
 * \addtogroup FileIO
 *
 * @brief Functions related to file I/O
 *
 * @{
 */
namespace birb
{
	/**
	 * @brief Generic file I/O related helper functions
	 */
	namespace io
	{
		/**
		 * @brief Read from a text file
		 *
		 * @warning If the file can't be opened for reading,
		 * things will crash and burn
		 *
		 * @param path Path to the file to be read
		 * @return File contents
		 */
		std::string read_file(const std::string& path);

		/**
		 * @brief Read from a file asynchronously
		 *
		 * @warning If the file can't be opened for reading,
		 * things will crash and burn
		 *
		 * @param path Path to the file to be read
		 * @return A future for the file contents
		 */
		std::future<std::string> read_file_async(const std::string& path);

		/**
		 * @brief Write to a text file
		 *
		 * @param path Path to the file to be written into
		 * @param text Text content that will be written into the file
		 * @return False if the file couldn't be opened for writing
		 */
		bool write_file(const std::string& path, const std::string& text);

		/**
		 * @brief Write to a text file asynchronously
		 *
		 * @param path Path to the file to be written into
		 * @param text Text content that will be written into the file
		 * @return A future to a boolean value that tells if the file writing was successful
		 */
		std::future<bool> write_file_async(const std::string& path, const std::string& text);

		/**
		 * @brief Read binary bson data from a file and convert that to json
		 *
		 * @param path Path to the binary file to be read
		 */
		nlohmann::json read_file_bson(const std::string& path);

		/**
		 * @brief Write a json file to disk
		 *
		 * @param path Path to the file to be written into
		 * @param json The json object that'll be written to the file
		 * @return False if the file couldn't be opened for writing
		 */
		bool write_json_file(const std::string& path, nlohmann::json json);

		/**
		 * @brief Write a bson file to disk
		 *
		 * @param path Path to the file to be written into
		 * @param json The json object that'll be converted to bson and written to the file
		 * @return False if the file couldn't be opened for writing
		 */
		bool write_bson_file(const std::string& path, nlohmann::json json);
	}
}
