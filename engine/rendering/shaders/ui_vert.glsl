#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vertex_color;

#include "include/matrices.glsl"

void main()
{
	gl_Position = projection_ortho_no_near_clip * vec4(position, 1.0);
}
