#pragma once

#ifndef NDEBUG

#include <cpptrace/cpptrace.hpp>

#define ASSERT(CONDITION)											\
	if (!(CONDITION))												\
	{																\
		std::cerr << "\n ! Assertion failed !\n\n";					\
		cpptrace::generate_trace().print_with_snippets(); abort();	\
	}

#define ASSERT_MSG(CONDITION, MSG)									\
	if (!(CONDITION))												\
	{																\
		std::cerr << "\n ! Assertion failed ! -> " << MSG << "\n\n";\
		cpptrace::generate_trace().print_with_snippets(); abort();	\
	}

#else

#define ASSERT(CONDITION)
#define ASSERT_MSG(CONDITION, MSG);

#endif // NDEBUG
