R"(
#version 330 core

in vec4 position;

uniform mat4 ms_matrix;

void main(void)
{
	gl_Position = ms_matrix * position;
}
)"