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
	print_limit_float<f32>("f32");
	print_limit_float<f64>("f64");
	print_limit_float<f128>("f128");

	return 0;
}
