#include "Random.hpp"

#include <doctest/doctest.h>
#include <iomanip>
#include <iostream>

TEST_CASE("RNG without seed")
{
	// Test two runs in a row to check if repeated calls
	// to the constructor are time sensitive

	birb::random rng;
	std::cout << "Random values: ";
	for (int i = 0; i < 10; ++i)
		std::cout << rng.next() % 255 << " ";
	std::cout << "\n";


	birb::random rng2;
	std::cout << "Random values: ";
	for (int i = 0; i < 10; ++i)
		std::cout << rng2.next() % 255 << " ";
	std::cout << "\n";
}

TEST_CASE("RNG with seed")
{
	birb::random rng(42);

	std::cout << "Random values with seed: ";
	for (int i = 0; i < 10; ++i)
		std::cout << rng.next() % 255 << " ";

	std::cout << "\n";
}

TEST_CASE("Random in range (seeded)")
{
	SUBCASE("Integer")
	{
		birb::random rng(42);
		std::cout << "Random values between 0 and 10: ";
		for (int i = 0; i < 10; ++i)
			std::cout << rng.range(0, 10) << " ";

		std::cout << "\n";
	}

	SUBCASE("Float")
	{
		birb::random rng(42);
		std::cout << "Random values between 0.0f and 10.0f: ";
		for (int i = 0; i < 8; ++i)
			std::cout << std::setprecision(3) << rng.range_float(0.0f, 10.0f) << " ";

		std::cout << "\n";
	}
}

TEST_CASE("Random vectors")
{
	birb::random rng(42);

	birb::vec2<int> rnd_vec2_int = rng.range_vec2_int(3, 7);
	CHECK(rnd_vec2_int.x >= 3);
	CHECK(rnd_vec2_int.x <= 7);
	CHECK(rnd_vec2_int.y >= 3);
	CHECK(rnd_vec2_int.y <= 7);

	birb::vec2<float> rnd_vec2_float = rng.range_vec2_float(3.0f, 7.0f);
	CHECK(rnd_vec2_float.x >= 3.0f);
	CHECK(rnd_vec2_float.x <= 7.0f);
	CHECK(rnd_vec2_float.y >= 3.0f);
	CHECK(rnd_vec2_float.y <= 7.0f);

	birb::vec3<int> rnd_vec3_int = rng.range_vec3_int(3, 7);
	CHECK(rnd_vec3_int.x >= 3);
	CHECK(rnd_vec3_int.x <= 7);
	CHECK(rnd_vec3_int.y >= 3);
	CHECK(rnd_vec3_int.y <= 7);
	CHECK(rnd_vec3_int.z >= 3);
	CHECK(rnd_vec3_int.z <= 7);

	birb::vec3<float> rnd_vec3_float = rng.range_vec3_float(3.0f, 7.0f);
	CHECK(rnd_vec3_float.x >= 3.0f);
	CHECK(rnd_vec3_float.x <= 7.0f);
	CHECK(rnd_vec3_float.y >= 3.0f);
	CHECK(rnd_vec3_float.y <= 7.0f);
	CHECK(rnd_vec3_float.z >= 3.0f);
	CHECK(rnd_vec3_float.z <= 7.0f);
}
