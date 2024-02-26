#include "Image.hpp"

namespace birb
{
	namespace asset
	{
		image::image(const char* path, bool flip_vertically)
		{
			assert(path != nullptr && "Invalid image path");

			stbi_set_flip_vertically_on_load(flip_vertically);

			data = stbi_load(path, &dimensions.x, &dimensions.y, &color_channels, 0);
			if (data == nullptr)
			{
				birb::log_error("Can't open an image at: " + std::string(path));
				return;
			}
		}

		image::~image()
		{
			stbi_image_free(data);
		}
	}
}
