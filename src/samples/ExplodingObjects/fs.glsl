#version 430

out vec4 color;

in VS_OUT
{
	vec4 normal;
} fs_in;

void main(void)
{
  float val = clamp(dot(fs_in.normal, vec4(0.70710678118654752440084436210485, 0.0, -0.70710678118654752440084436210485, 0.0)), -1.0, -0.4) * (-1.0);
  color = vec4(255.0 / 256.0 * val, 239.0 / 256.0 * val, 213.0 / 256.0 * val, 1.0);
}
