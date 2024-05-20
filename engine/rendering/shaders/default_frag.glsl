#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 object_color;
uniform int point_light_count;

uniform vec3 view_pos;

#define POINT_LIGHT_COUNT 64
#define TEXTURE
#include "include/lights.glsl"

void main()
{
	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(view_pos - FragPos);

	// Directional lighting
	vec3 result = CalcDirLight(directional_light, norm, view_dir);

	// Loop over point lights
	for (int i = 0; i < point_light_count; i++)
		result += CalcPointLight(point_lights[i], norm, FragPos, view_dir);

	FragColor = vec4(result, 1.0f);
}
