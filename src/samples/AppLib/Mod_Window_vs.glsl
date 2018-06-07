R"(
#version 330 core

in vec2 in_position;
in vec2 in_texCoords;

out VertexData
{
  vec2 texCoords;
} o;

void main()
{
    gl_Position = vec4(in_position.x, in_position.y, 0.0, 1.0); 
    o.texCoords = in_texCoords;
}
)"