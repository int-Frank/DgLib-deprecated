#version 430 core

in vec4 position;
in vec4 color;

out VS_OUT
{
	vec4 color;	
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * position;
	vs_out.color = color;
  gl_PointSize = 20.0;
}
