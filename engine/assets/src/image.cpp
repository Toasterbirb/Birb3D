#include "Assert.hpp"
#include "Image.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"

#include <stb_image.h>

namespace birb
{
	namespace asset
	{
		image::image(const char* path, bool flip_vertically)
		{
			PROFILER_SCOPE_IO_FN();
			ASSERT(path != nullptr && "Invalid image path");

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
