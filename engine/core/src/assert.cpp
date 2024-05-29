#include "Assert.hpp"

#include <iostream>

#ifndef NDEBUG
#include <cpptrace/cpptrace.hpp>
#endif

namespace birb
{
	void ensure(bool condition)
	{
#ifndef NDEBUG
		if (!(condition))
		{
			std::cerr << "\n ! Assertion failed !\n\n";
			cpptrace::generate_trace(1).print_with_snippets(); abort();
		}
#endif
	}

	void ensure(bool condition, const char* msg)
	{
#ifndef NDEBUG
		if (!(condition))
		{
			std::cerr << "\n ! Assertion failed ! -> " << msg << "\n\n";
			cpptrace::generate_trace(1).print_with_snippets(); abort();
		}
#endif
	}
}
