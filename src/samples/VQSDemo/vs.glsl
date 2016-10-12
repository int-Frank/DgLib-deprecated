#version 330 core

in vec4 position;
in vec4 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out VS_OUT
{
	vec4 normal;	
} vs_out;

void main(void)
{
  gl_Position = proj_matrix * mv_matrix * position;

  vs_out.normal = normalize(mv_matrix * vec4(normal.x, normal.y, normal.z, 0.0));
}
