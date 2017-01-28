#version 430 core

in vec4 position;
in vec4 normal;
in int  index;

// Spaces...
// M : Model
// W : World
// V : View
// S : Screen

uniform mat4 T_W_M;
uniform mat4 T_V_W;
uniform mat4 T_S_V;
uniform float totalTime;

struct TransformData
{
  vec4 translation;
  vec4 rotation;
};

layout(std430) buffer physics_data
{
  TransformData movement_rates[];
};


void BuildMatrix(in int a_index, out mat4 result);
void BuildMatrix(in int a_index, out mat4 result)
{
  float rx = totalTime * movement_rates[a_index].rotation.x;
  float ry = totalTime * movement_rates[a_index].rotation.y;
  float rz = totalTime * movement_rates[a_index].rotation.z;
  
  result = mat4(1.0);
  
  //Rotation
  float Cx = cos(rx);
  float Sx = sin(rx);
  
  float Cy = cos(ry);
  float Sy = sin(ry);
  
  float Cz = cos(rz);
  float Sz = sin(rz);
  
  result[0][0] = Cy * Cz;
  result[0][1] = (Sz * Cx) + (Sy * Sx * Cz);
  result[0][2] = (Sz * Sx) - (Sy * Cx * Cz);
  result[0][3] = 0.0;
  
  result[1][0] = -(Sz * Cy);
  result[1][1] = (Cx * Cz) - (Sz * Sy * Sx);
  result[1][2] = (Sz * Sy * Cx) + (Sx * Cz);
  result[1][3] = 0.0;
  
  result[2][0] = Sy;
  result[2][1] = -(Sx * Cy);
  result[2][2] = Cx * Cy;
  result[2][3] = 0.0;
  
  //Translation
  
  vec4 translation = totalTime * movement_rates[a_index].translation;
  
  result[3][0] = translation.x;
  result[3][1] = translation.y;
  result[3][2] = translation.z;
  result[3][3] = 1.0;
}

out VS_OUT
{
	vec4 normal;	
} vs_out;

void main(void)
{
  mat4 W2W1;
  BuildMatrix(index, W2W1);
  gl_Position = T_S_V * T_V_W * W2W1 * T_W_M * position;

  vs_out.normal = normalize(T_V_W * T_W_M * vec4(normal.x, normal.y, normal.z, 0.0));
}