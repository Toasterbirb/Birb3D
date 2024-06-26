#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

#include "include/matrices.glsl"

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoord = aTex;
}
