#version 330 core
out vec3 FragColor;

uniform vec3 light_color;

void main()
{
	FragColor = light_color; // White color
}
