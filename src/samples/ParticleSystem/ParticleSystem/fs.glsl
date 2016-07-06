#version 430

out vec4 color;

in VS_OUT
{
	vec4 color;
} fs_in;

void main(void)
{
  float distSqrd = dot(gl_PointCoord-0.5,gl_PointCoord-0.5);
  if(distSqrd > 0.25) 
  {
    discard;
  }
  else
  {
    color = fs_in.color;
    color.w = color.w * (1.0 - distSqrd / 0.25);
  }
}
