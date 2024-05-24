uniform mat4 model;

layout (std140) uniform view_matrices
{
	uniform mat4 view;
};

layout (std140) uniform projection_matrices
{
	uniform mat4 projection;
	uniform mat4 projection_ortho;
	uniform mat4 projection_ortho_no_near_clip;
};
