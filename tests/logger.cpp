#include "Logger.hpp"
#include <doctest/doctest.h>

TEST_CASE("Logging")
{
	birb::log("Logging test");
	birb::log_warn("Warning test");
	birb::log_error("Error test");
	fixme("Fixme test");
}
