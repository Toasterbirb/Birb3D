#version 330 core
layout (location = 0) in vec4 vertex; // vec2 position | vec2 texture_coord
out vec2 TexCoords;

uniform vec2 glyph_position;
uniform mat4 custom_projection;

#include "include/matrices.glsl"

void main()
{
	gl_Position = custom_projection * vec4(vertex.xy + glyph_position.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}
