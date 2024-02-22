#include "Color.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./hex_to_float FF00AA\n";
		return 1;
	}

	birb::color color(std::stoi(argv[1], 0, 16));

	std::cout << color.r << " " << color.g << " " << color.b << "\n";

	return 0;
}
