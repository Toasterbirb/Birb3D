#include <doctest/doctest.h>
#include <filesystem>
#include <future>
#include <string>

#include "IO.hpp"

TEST_CASE("File reading and writing")
{
	const std::string path = "/tmp/birb3d_file_io_test";
	const std::string text = "This is a test string used in the Birb3D tests";

	const std::string path_async = "/tmp/birb3d_file_io_test_async";
	const std::string text_async = "This is a test string used in the Birb3D async tests";

	// Make sure no files exist at /tmp/birb3d_file_io_test nor /tmp/birb3d_file_io_test_async
	std::filesystem::remove(path);
	std::filesystem::remove(path_async);

	// Write a test string into a file
	birb::io::write_file(path, text);

	// Write a test string asynchronously into a file
	std::future<bool> async_write_future = birb::io::write_file_async(path_async, text_async);

	// Try to read the text from a file
	std::string result = birb::io::read_file(path);

	// Try to read the text from a file asynchronously
	std::future<std::string> result_future = birb::io::read_file_async(path_async);

	CHECK(result == text);
	CHECK(result_future.get() == text_async);
	CHECK(async_write_future.get());
}
