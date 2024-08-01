#version 330 core

in vec2 texCoords;
in vec3 FragPos;

uniform float time;
uniform vec3 color;

void main()
{
	vec2 uv = FragPos.xz;

	vec2 uv0 = uv;
	vec4 final_color = vec4(0.0);

	uv *= 0.5;

	for (float i = 0; i < 5.0; i++)
	{
		uv = fract(cos(uv) / 5.);
		uv -= 0.5;

		float d = length(uv);

		d = sin((1 / d) * time);
		d = abs(d);
		d = 0.02 / d;

		final_color += vec4(color * d, d);
	}

	final_color = clamp(final_color, 0.0, 0.8);

	gl_FragColor = vec4(final_color);
}
