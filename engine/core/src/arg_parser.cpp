#include "ArgParser.hpp"
#include "Assert.hpp"
#include "Logger.hpp"
#include "Types.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace birb
{
	arg_parser::arg_parser()
	{
		// Add some default options
		add_option("h", true, "Same as --help");
		add_option("help", true, "Print the help text you are reading right now");
		add_option("version", true, "Show version information and quit");
	}

	std::string arg_parser::executable_path() const
	{
		return _executable_path;
	}

	void arg_parser::add_option(const std::string& key, const bool is_boolean, const std::string& help)
	{
		ensure(!options.contains(key), "Duplicate option");
		options[key] = { is_boolean, help };
	}

	void arg_parser::process_arguments(const int argc, char** argv)
	{
		_executable_path = argv[0];

		std::vector<std::string> args(argc - 1);

		// We'll skip the first argument since its always just the path
		// to the executable file
		for (i32 i = 1; i < argc; ++i)
			args[i - 1] = argv[i];

		// Process the arguments as std::strings
		for (size_t i = 0; i < args.size(); ++i)
		{
			// Skip invalid options //

			// Prevents: a
			if (args[i].size() < 2)
				invalid_option_error(args[i]);

			// Prevents: a-
			if (args[i][0] != '-')
				invalid_option_error(args[i]);

			// Prevents: --
			if (args[i] == "--")
				invalid_option_error(args[i]);

			// Prevents: -version
			if (args[i].size() > 2 && args[i][1] != '-')
				invalid_option_error(args[i]);

			// Prevents: ---version and -------version etc.
			if (args[i].size() >= 3 && args[i].substr(0, 3) == "---")
				invalid_option_error(args[i]);

			// Trim the dashes from the beginning of the option
			const size_t first_non_dash_char = args[i].find_first_not_of("-");
			const std::string original_option = args[i];
			args[i].erase(0, first_non_dash_char);

			if (!options.contains(args[i]))
				invalid_option_error(original_option);

			// Check if the option requires a value
			if (options.at(args[i]).first == true)
			{
				if (process_reserved_boolean_args(args[i]))
					continue;

				parsed_boolean_args.insert(args[i]);
				continue;
			}

			// Check if there are enough arguments left for a value
			if (i >= args.size() - 1)
				log_fatal(1, "Option ", original_option, " requires a value");

			if (process_reserved_key_value_args(args[i], args[i + 1]))
				continue;

			parsed_args_with_value[args[i]] = args[i + 1];

			// Skip the next argument since it is a value
			i += 1;
		}
	}

	bool arg_parser::is_defined(const std::string& key) const
	{
		return (parsed_args_with_value.contains(key) || parsed_boolean_args.contains(key));
	}

	std::string arg_parser::get_value(const std::string& key) const
	{
		ensure(!parsed_boolean_args.contains(key), "Tried to fetch for a value from a boolean key");
		return parsed_args_with_value.at(key);
	}

	void arg_parser::invalid_option_error(const std::string& option)
	{
		birb::log_fatal(1, "Invalid argument: ", option);
	}

	bool arg_parser::process_reserved_boolean_args(const std::string& option) const
	{
		if (option == "h" || option == "help")
		{
			print_help();
			exit(0);
		}

		if (option == "version")
		{
			print_version();
			exit(0);
		}

		return false;
	}

	bool arg_parser::process_reserved_key_value_args(const std::string& option, const std::string& value) const
	{
		return false;
	}

	void arg_parser::print_help() const
	{
		std::cout	<< "Usage: " << _executable_path << " [OPTIONS]\n";

		if (!description.empty())
			std::cout << description << "\n";

		std::cout	<< "\n"
					<< "Options:\n";

		constexpr i16 padding = 24;
		const std::string opt_value_str = " [value] ";

		for (const auto& [option, opt] : options)
		{
			// Some indenting
			std::string option_str = "  ";

			// Add the dashes to the beginning of the option
			if (option.size() == 1)
				option_str += "-";
			else
				option_str += "--";

			option_str += option;
			ensure(padding - option_str.size() > 0);

			// Handle boolean options
			if (opt.first)
			{
				std::cout 	<< option_str
							<< std::setw(padding - option_str.size())
							<< " " << opt.second;
				std::cout << "\n";
			}

			// Handle non-boolean options
			if (!opt.first)
			{
				std::cout 	<< std::left
							<< option_str
							<< std::setw(padding - option_str.size())
							<< opt_value_str
						 	<< opt.second;
				std::cout << "\n";
			}
		}
	}

	void arg_parser::print_version() const
	{
		std::cout 	<< "Date: " << __DATE__ << "\n"
					<< "Time: " << __TIME__ << "\n";
#ifndef NDEBUG
		std::cout << "Contains debug symbols\n";
#endif

#ifdef BIRB_RELEASE
		std::cout << "Dist build with reduced debugging features\n";
#endif
	}
}
