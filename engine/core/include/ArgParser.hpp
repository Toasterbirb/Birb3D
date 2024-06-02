#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace birb
{
	class arg_parser
	{
	public:
		arg_parser();
		~arg_parser() = default;
		arg_parser(const arg_parser&) = default;
		arg_parser(arg_parser&) = default;

		std::string description;
		std::string executable_path() const;

		void add_option(const std::string& key, const bool is_boolean, const std::string& help);
		void process_arguments(const int argc, char** argv);

		bool is_defined(const std::string& key) const;
		std::string get_value(const std::string& key) const;

	private:
		std::string _executable_path;

		// std::string key, pair<is_boolean, help_text>
		std::map<std::string, std::pair<bool, std::string>> options;

		std::unordered_map<std::string, std::string> parsed_args_with_value;
		std::unordered_set<std::string> parsed_boolean_args;

		/**
		 * @brief Print an error message and abort
		 */
		void invalid_option_error(const std::string& option);

		/**
		 * @brief Process pre-defined boolean arguments used by Birb3D
		 *
		 * @return True if a pre-defined argument was processed
		 */
		bool process_reserved_boolean_args(const std::string& option) const;

		/**
		 * @brief Process pre-defined key-value arguments used by Birb3D
		 *
		 * @return True if a pre-defined argument was processed
		 */
		bool process_reserved_key_value_args(const std::string& option, const std::string& value) const;

		void print_help() const;
		void print_version() const;
	};
}
