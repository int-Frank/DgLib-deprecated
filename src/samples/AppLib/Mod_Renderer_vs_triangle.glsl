R"(
#version 330

in vec4 in_position;
in vec4 in_normal;
in vec2 in_uv;

uniform mat4 u_mvp;

void main(void)
{
	gl_Position = u_mvp * in_position;
}
)"