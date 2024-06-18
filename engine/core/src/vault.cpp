#include "IO.hpp"
#include "Logger.hpp"
#include "Vault.hpp"

#include <cstdlib>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace birb
{
	/**
	 * @brief Keep track of opened vaults
	 *
	 * This is to avoid conflicts by having the same
	 * vault open in multiple places
	 */
	static std::unordered_set<std::string> file_lock;

	vault::vault(const std::string& application_name)
	{
#ifdef BIRB_PLATFORM_WINDOWS
		ensure(!std::string(getenv("APPDATA")).empty());
		const std::string vault_dir_path = std::string(getenv("APPDATA")) + "/birb3d";
#else
		ensure(!std::string(getenv("HOME")).empty());
		const std::string vault_dir_path = std::string(getenv("HOME")) + "/.local/share/birb3d";
#endif

		if (!std::filesystem::exists(vault_dir_path))
			std::filesystem::create_directories(vault_dir_path);

		const std::string vault_path = vault_dir_path + "/" + application_name + ".json";
		this->file_path = vault_path;

		// Check if the file is already opened
		if (file_lock.contains(file_path))
			birb::log_fatal(10, "Only a singular instance of a vault can exist at any given time");

		// Create an empty vault file
		_is_new = false;
		if (!std::filesystem::exists(vault_path))
		{
			io::write_file(vault_path, "{}");
			_is_new = true;
		}

		const std::string json_str = io::read_file(vault_path);
		json = nlohmann::json::parse(json_str);

		ensure(!file_path.empty());
		file_lock.insert(file_path);
	}

	vault::~vault()
	{
		// "Unlock" the vault file so that it can be opened again by
		// another vault instance
		file_lock.erase(file_path);
	}

	void vault::clear()
	{
		json.clear();
	}

	void vault::save()
	{
		io::write_json_file(file_path, json);
	}

	bool vault::is_new() const
	{
		return _is_new;
	}

	bool vault::contains(const std::string& key, const std::string& category) const
	{
		return json.contains(category) && json.at(category).contains(key);
	}
}
