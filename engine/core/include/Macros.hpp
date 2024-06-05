#pragma once

////////////////////////////////////////////////////////////////////////////
// This file is supposed to contain generic macros for different purposes //
////////////////////////////////////////////////////////////////////////////


// Some fancy macros from https://stackoverflow.com/a/17624752
// for generating unique_variable names
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)
