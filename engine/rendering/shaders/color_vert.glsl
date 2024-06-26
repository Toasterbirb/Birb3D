#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

#include "include/matrices.glsl"

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
}
