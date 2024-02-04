#include <doctest/doctest.h>
#include <filesystem>
#include <future>
#include <string>

#include "IO.hpp"

TEST_CASE("File reading and writing")
{
	const std::string path = "/tmp/birb3d_file_io_test";
	const std::string text = "This is a test string used in the Birb3D tests";

	// Make sure no files exist at /tmp/birb3d_file_io_test
	std::filesystem::remove(path);

	// Write a test string into the file
	birb::io::write_file(path, text);

	// Try to read the text from the file
	std::string result = birb::io::read_file(path);

	// Try to read the text from the file asynchronously
	std::future<std::string> result_future = birb::io::read_file_async(path);

	CHECK(result == text);
	CHECK(result_future.get() == text);
}
