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

	SUBCASE("Inclusivity test")
	{
		birb::random rng(42);
		bool found_min_value = false;
		bool found_max_value = false;

		// Loop until both bounds have been found by random chance
		std::cout << "Testing if the random int function is inclusive\n";
		std::cout << "If the test hangs, there's a bug that needs fixing\n";
		while (!found_min_value || !found_max_value)
		{
			constexpr int min = 1;
			constexpr int max = 5;
			const int random_value = rng.range(min, max);

			if (random_value == min)
				found_min_value = true;

			if (random_value == max)
				found_max_value = true;
		}

		CHECK(found_min_value);
		CHECK(found_max_value);
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

TEST_CASE("Shuffle a vector of integers")
{
	constexpr u8 value_count = 128;
	std::vector<u8> ints_original;
	std::vector<u8> ints;

	for (u8 i = 0; i < value_count; ++i)
		ints_original.push_back(i);

	birb::random rng(42);

	ints = rng.shuffle(ints_original);

	CHECK_FALSE(ints == ints_original);
}

TEST_CASE("Shuffle a vector of integers in-place")
{
	constexpr u8 value_count = 128;
	std::vector<u8> ints_original;
	std::vector<u8> ints;

	for (u8 i = 0; i < value_count; ++i)
		ints_original.push_back(i);

	ints = ints_original;
	CHECK(ints == ints_original);

	birb::random rng(42);
	rng.shuffle_in_place(ints);

	CHECK_FALSE(ints == ints_original);
}

TEST_CASE("Shuffle an array of integers")
{
	constexpr u8 value_count = 128;
	std::array<u8, value_count> ints_original;
	std::array<u8, value_count> ints;

	for (u8 i = 0; i < value_count; ++i)
		ints_original.at(i) = i;

	birb::random rng(42);

	ints = rng.shuffle(ints_original);

	CHECK_FALSE(ints == ints_original);
}

TEST_CASE("Shuffle an array of integers in-place")
{
	constexpr u8 value_count = 128;
	std::array<u8, value_count> ints_original;
	std::array<u8, value_count> ints;

	for (u8 i = 0; i < value_count; ++i)
		ints_original.at(i) = i;

	ints = ints_original;
	CHECK(ints == ints_original);

	birb::random rng(42);
	rng.shuffle_in_place(ints);

	CHECK_FALSE(ints == ints_original);
}
