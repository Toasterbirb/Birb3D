#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex_coords;
out vec2 TexCoords;

#include "include/matrices.glsl"

uniform vec3 text_position;

void main()
{
	gl_Position = vec4(vertex.x, -vertex.y, 0.0, 1.0);
	TexCoords = tex_coords;
}
