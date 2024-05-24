#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 5) in mat4 instanceMatrix;

out vec2 texCoord;

#include "include/matrices.glsl"

uniform int instanced;

uniform vec2 aspect_ratio;
// aspect_ratio.x = aspect_ratio
// aspect_ratio.y = aspect_ratio_reverse

void main()
{
	if (instanced == 1)
		gl_Position = projection_ortho * view * instanceMatrix * vec4(aPos.x / aspect_ratio.y, aPos.y / aspect_ratio.x, aPos.z, 1.0f);
	else
		gl_Position = projection_ortho * view * model * vec4(aPos.x / aspect_ratio.y, aPos.y / aspect_ratio.x, aPos.z, 1.0f);

	texCoord = aTex;
}
