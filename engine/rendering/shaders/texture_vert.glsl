#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

#include "include/matrices.glsl"

uniform float aspect_ratio;
uniform float aspect_ratio_reverse;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x / aspect_ratio_reverse, aPos.y / aspect_ratio, aPos.z, 1.0f);
	texCoord = aTex;
}
