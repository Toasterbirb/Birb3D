#version 330 core
layout (location = 0) in vec4 vertex; // vec2 position | vec2 texture_coord
layout (location = 1) in vec2 vertex_offset;
out vec2 TexCoords;

uniform vec3 text_position;
//uniform mat4 projection;

#include "include/matrices.glsl"

void main()
{
	gl_Position = projection_ortho_no_near_clip * vec4(vertex.xy + vertex_offset + text_position.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}
