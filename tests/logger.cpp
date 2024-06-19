#include "Logger.hpp"
#include <doctest/doctest.h>

TEST_CASE("Logging")
{
	birb::log("Logging test");
	birb::log("Some number: ", 10, ", hello ", "world");
	birb::log_warn("Warning test: ", 0);
	birb::log_error_no_trace("Error test: ", 42);
	FIXME("Fixme test");
}
