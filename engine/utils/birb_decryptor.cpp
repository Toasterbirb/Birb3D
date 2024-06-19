#include "IO.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: birb_decryptor ./path/to/obfuscated/file\n";
		return 1;
	}

	std::string plaintext = birb::io::read_file(argv[1]);
	std::cout << plaintext;

	// Print a newline character if the file doesn't have a newline at the end
	if (*(plaintext.end() - 1) != '\n')
		std::cout << '\n';
}
