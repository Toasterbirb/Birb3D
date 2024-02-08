#include "Random.hpp"

#include <doctest/doctest.h>
#include <iomanip>
#include <iostream>

TEST_CASE("RNG without seed")
{
	// Test two runs in a row to check if repeated calls
	// to the constructor is time sensitive

	birb::random rng;
	std::cout << "Random values: ";
	for (int i = 0; i < 20; ++i)
		std::cout << rng.next() % 10 << " ";
	std::cout << "\n";


	birb::random rng2;
	std::cout << "Random values: ";
	for (int i = 0; i < 20; ++i)
		std::cout << rng2.next() % 10 << " ";
	std::cout << "\n";
}

TEST_CASE("RNG with seed")
{
	birb::random rng(42);

	std::cout << "Random values with seed: ";
	for (int i = 0; i < 20; ++i)
		std::cout << rng.next() % 10 << " ";

	std::cout << "\n";
}

TEST_CASE("Random in range (seeded)")
{
	SUBCASE("Integer")
	{
		birb::random rng(42);
		std::cout << "Random values between 0 and 10: ";
		for (int i = 0; i < 20; ++i)
			std::cout << rng.range(0, 10) << " ";

		std::cout << "\n";
	}

	SUBCASE("Float")
	{
		birb::random rng(42);
		std::cout << "Random values between 0.0f and 10.0f: ";
		for (int i = 0; i < 10; ++i)
			std::cout << std::setprecision(3) << rng.range_float(0.0f, 10.0f) << " ";

		std::cout << "\n";
	}
}
