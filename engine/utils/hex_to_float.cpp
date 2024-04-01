#include "Types.hpp"
#include "Color.hpp"

#include <string.h>
#include <iostream>

enum mode {
	vec, def
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: ./hex_to_float FF00AA\n";
		return 1;
	}

	/* Different modes */
	mode mode = mode::def;
	if (argc == 3)
	{
		if (!strcmp(argv[2], "vec"))
			mode = mode::vec;
	}

	birb::color color(std::stoi(argv[1], 0, 16));

	switch (mode)
	{
		case def:
			std::cout << color.r << " " << color.g << " " << color.b << "\n";
			break;

		case vec:
			std::cout << color.r << "f, " << color.g << "f, " << color.b << "f, 1.0f\n";
			break;
	}

	return 0;
}
