#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text_atlas;

void main()
{
	FragColor = texture(text_atlas, TexCoords);
}
