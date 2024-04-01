#version 330 core
out vec4 FragColor;

in vec3 color;

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	vec4 tex_color = texture(tex0, texCoord);
	if (tex_color.a < 0.1)
		discard;

	FragColor = tex_color;
}
