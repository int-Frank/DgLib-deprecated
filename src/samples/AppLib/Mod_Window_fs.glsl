R"(
#version 330 core

in VertexData
{
  vec2 texCoords;
} i;

out vec4 out_color;
  
uniform sampler2D screenTexture;

void main()
{ 
    out_color = texture(screenTexture, i.texCoords);
}
)"