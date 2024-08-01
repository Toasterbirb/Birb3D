#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 5) in mat4 instanceMatrix;

out vec2 texCoord;
out vec3 FragPos;


#include "include/matrices.glsl"

uniform int instanced;
uniform int orthographic;

uniform vec2 aspect_ratio;
// aspect_ratio.x = aspect_ratio
// aspect_ratio.y = aspect_ratio_reverse

void main()
{
	mat4 projection_matrix;
	if (orthographic == 1)
		projection_matrix = projection_ortho;
	else
		projection_matrix = projection;

	if (instanced == 1)
		gl_Position = projection_matrix * view * instanceMatrix * vec4(aPos.x / aspect_ratio.y, aPos.y / aspect_ratio.x, aPos.z, 1.0f);
	else
		gl_Position = projection_matrix * view * model * vec4(aPos.x / aspect_ratio.y, aPos.y / aspect_ratio.x, aPos.z, 1.0f);

	texCoord = aTex;
	FragPos = vec3(model * vec4(aPos, 1.0));
}
