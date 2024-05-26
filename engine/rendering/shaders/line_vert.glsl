#version 330 core
layout (location = 0) in vec3 aPos;

#include "include/matrices.glsl"

void main()
{
	gl_Position = projection_ortho_no_near_clip * vec4(aPos, 1.0);
}
