#include "Profiling.hpp"
#include "Sprite.hpp"

namespace birb
{
	sprite::sprite(const std::string& file_path, color_format format)
	{
		PROFILER_SCOPE_RENDER_FN()

		texture = std::make_shared<birb::texture>(file_path.c_str(), 0, format, 2);
	}
}
