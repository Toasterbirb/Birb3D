#include "Types.hpp"
#include "Vault.hpp"
#include "Vector.hpp"

#include <doctest/doctest.h>
#include <vector>

static constexpr char vault_name[] = "birb3d_unit_test_vault";

TEST_CASE("Vault")
{
	birb::vault vault(vault_name);

	// Clear the vault
	vault.clear();
	vault.save(true);

	SUBCASE("Set and get an integer")
	{
		const std::string key = "number";
		const std::string category = "values";
		const i32 value = 42;

		CHECK_FALSE(vault.contains(key, category));

		vault.set(value, key, category);

		CHECK(vault.contains(key, category));
		CHECK(vault.get<i32>(key, category) == value);
	}

	SUBCASE("Set and get a vec2")
	{
		const std::string key = "vec2";
		const std::string category = "values";
		const birb::vec2<u16> vec(21, 94);

		CHECK_FALSE(vault.contains(key, category));

		vault.set_vec2(vec, key, category);

		CHECK(vault.contains(key, category));
		CHECK(vault.get_vec2<u16>(key, category) == vec);
	}

	SUBCASE("Set and get a vec3")
	{
		const std::string key = "vec3";
		const std::string category = "values";
		const birb::vec3<u16> vec(21, 94, 35);

		CHECK_FALSE(vault.contains(key, category));

		vault.set_vec3(vec, key, category);

		CHECK(vault.contains(key, category));
		CHECK(vault.get_vec3<u16>(key, category) == vec);
	}

	SUBCASE("Set and get a vector of integers")
	{
		const std::string key = "vector";
		const std::string category = "values";
		const std::vector<i32> values = { 1, 2, 3, 4, 5 };

		CHECK_FALSE(vault.contains(key, category));

		vault.set(values, key, category);

		CHECK(vault.contains(key, category));
		CHECK(vault.get<std::vector<i32>>(key, category) == values);
	}

	SUBCASE("Get a non-existent value")
	{
		const std::string key = ":(";
		const std::string category = "values";
		const u8 fallback_value = 42;

		CHECK_FALSE(vault.contains(key, category));
		CHECK(vault.get<u8>(fallback_value, key, category) == fallback_value);
	}

	// Save the changes to make sure that the reading / clearing works
	vault.save(true);
}

TEST_CASE("Vault saving and loading")
{
	const std::string& key = "key";
	const std::string& category = "category";
	const birb::vec2<i32> reset_value(0, 0);
	const birb::vec2<i32> new_value(42, 24);
	const bool obfuscation_enabled = true;

	// Reset the value
	{
		birb::vault vault(vault_name);
		vault.set_vec2<i32>(reset_value, key, category);
		CHECK(vault.get_vec2<i32>(key, category) == reset_value);
		vault.save(obfuscation_enabled);
	}

	// Attempt to read the reset value
	{
		birb::vault vault(vault_name);
		CHECK_FALSE(vault.is_new());
		CHECK(vault.get_vec2<i32>(key, category) == reset_value);
	}

	// Change the reset value to a new value
	{
		birb::vault vault(vault_name);
		CHECK_FALSE(vault.is_new());
		vault.set_vec2<i32>(new_value, key, category);
		CHECK(vault.get_vec2<i32>(key, category) == new_value);
		vault.save(obfuscation_enabled);
	}

	// Make sure that the value changed
	{
		birb::vault vault(vault_name);
		CHECK_FALSE(vault.is_new());
		CHECK(vault.get_vec2<i32>(key, category) == new_value);
	}
}
