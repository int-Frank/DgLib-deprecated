#version 330 core

in vec4 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
  float sx = length(mv_matrix[0]);
  float sy = length(mv_matrix[1]);
  float sz = length(mv_matrix[2]);

  position.x = position.x * sx;
  position.y = position.y * sy;
  position.z = position.z * sz;
  
	gl_Position = proj_matrix * mv_matrix * position;

}
