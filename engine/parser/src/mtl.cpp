#include "IO.hpp"
#include "MTL.hpp"

#include <cstdio>
#include <sstream>
#include <string>

namespace birb
{
	namespace parser
	{
		std::unordered_map<std::string, material> mtl(const std::string& mtl_file_path)
		{
			std::unordered_map<std::string, material> materials;

			const std::string data = io::read_file(mtl_file_path);

			// Loop through the file line by line
			std::stringstream ss(data);
			std::string current_material = "";

			for (std::string line; std::getline(ss, line, '\n');)
			{
				// Ignore comments and empty lines
				if (line.empty() || line.at(0) == '#')
					continue;

				// Set the current material
				if (line.starts_with("newmtl"))
				{
					size_t whitespace_pos = line.find_first_of(' ');
					current_material = line.substr(whitespace_pos + 1, line.size() - whitespace_pos + 1);
					std::cout << "Material: " << current_material << "\n";
					continue;
				}

				// Read diffuse color
				if (line.starts_with("Kd"))
				{
					color& diffuse = materials[current_material].diffuse;
					sscanf(line.c_str(), "Kd %f %f %f", &diffuse.r, &diffuse.g, &diffuse.b);
					continue;
				}

				// Read specular color
				if (line.starts_with("Ks"))
				{
					color& specular = materials[current_material].specular;
					sscanf(line.c_str(), "Ks %f %f %f", &specular.r, &specular.g, &specular.b);
					continue;
				}

				// Read shininess
				if (line.starts_with("Ns"))
				{
					sscanf(line.c_str(), "Ns %f", &materials[current_material].shininess);
					continue;
				}
			}

			return materials;
		}
	}
}
