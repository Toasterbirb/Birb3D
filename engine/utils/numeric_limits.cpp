#include "Types.hpp"

#include <iostream>
#include <limits>

template<typename T>
void print_limit(const std::string_view& name)
{
	constexpr i64 min_value = std::numeric_limits<T>().min();
	constexpr u64 max_value = std::numeric_limits<T>().max();
	std::cout << name << ":\t" << min_value << " -> " << max_value << "\n";
}

template<typename T>
void print_limit_float(const std::string_view& name)
{
	constexpr long double min_value = std::numeric_limits<T>().min();
	constexpr long double max_value = std::numeric_limits<T>().max();
	std::cout << name << ":\t" << min_value << " -> " << max_value << "\n";
}

int main(void)
{
	print_limit<i8>("i8");
	print_limit<i16>("i16");
	print_limit<i32>("i32");
	print_limit<i64>("i64");
	std::cout << "\n";
	print_limit<u8>("u8");
	print_limit<u16>("u16");
	print_limit<u32>("u32");
	print_limit<u64>("u64");
	std::cout << "\n";
	print_limit_float<f16>("f16");
	print_limit_float<f32>("f32");
	print_limit_float<f64>("f64");

	// Testing for the size of f128 doesn't work directly
	// so use the equivalent long double instead
	// (assuming that they are the same size)
	static_assert(sizeof(f128) == sizeof(long double));
	print_limit_float<long double>("f128");

	print_limit_float<bf16>("bf16");

	return 0;
}
