#pragma once

namespace birb
{
	struct character
	{
		u32			texture_id; // Handle to the OpenGL texture
		vec2<u32>	size;		// Dimensions of the glyph
		vec2<i32>	bearing;	// Offset from baseline to top left corner of a glyph
		i64			advance;	// Offset to the next glyph
	};
}
