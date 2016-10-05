#version 430 core

in vec4 position;

out VS_OUT
{
	vec4 color;	
} vs_out;

//uniform int       nTypes;
//uniform int[256]  IDs;
//uniform vec4[256] colors;

void main(void)
{
	gl_Position = position;
  vs_out.color = vec4(1.0, 0.0, 1.0, 1.0);
}
