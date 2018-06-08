
#include <cmath>

#include "ShapeFunctions.h"

Renderer::LineMesh GenerateLineStar()
{
  float pi = 3.14159f;

  int n_points = 6;
  float radius = 0.5f;
  float depth = 0.25f;
  float spacing = 2.0f * pi / float(n_points);
  float halfSpacing = spacing / 2.0f;

  Renderer::LineMesh result;

  for (int i = 0; i < n_points; i++)
  {
    Renderer::LineVertex point, dimple;

    point.point[0] = radius * cos(spacing * float(i));
    point.point[1] = radius * sin(spacing * float(i));
    point.point[2] = 0.0f;

    dimple.point[0] = depth * cos(spacing * float(i) + halfSpacing);
    dimple.point[1] = depth * sin(spacing * float(i) + halfSpacing);
    dimple.point[2] = 0.0f;

    result.verts.push_back(point);
    result.verts.push_back(dimple);
  }

  for (int i = 0; i < n_points * 2; i++)
  {
    Renderer::Line line;
    line.indices[0] = i;
    line.indices[1] = (i + 1) % (n_points * 2);
    result.lines.push_back(line);
  }

  return result;
}

Renderer::LineMesh GenerateLineBox()
{
  Renderer::LineMesh result;
  Renderer::LineVertex point;

  point.point[2] = 0.0f;

  point.point[0] = -0.5f;
  point.point[1] = -0.5f;
  result.verts.push_back(point);

  point.point[0] = 0.5f;
  point.point[1] = -0.5f;
  result.verts.push_back(point);

  point.point[0] = 0.5f;
  point.point[1] = 0.5f;
  result.verts.push_back(point);

  point.point[0] = -0.5f;
  point.point[1] = 0.5f;
  result.verts.push_back(point);

  Renderer::Line line;

  line.indices[0] = 0;
  line.indices[1] = 1;
  result.lines.push_back(line);

  line.indices[0] = 1;
  line.indices[1] = 2;
  result.lines.push_back(line);

  line.indices[0] = 2;
  line.indices[1] = 3;
  result.lines.push_back(line);

  line.indices[0] = 3;
  line.indices[1] = 0;
  result.lines.push_back(line);

  return result;
}

Renderer::LineMesh GenerateLineDebugPattern()
{
  Renderer::LineMesh result;
  Renderer::LineVertex point;

  point.point[2] = 0.0f;

  //Square
  point.point[0] = -0.1f;
  point.point[1] = 0.6f;
  result.verts.push_back(point);

  point.point[0] = 0.1f;
  point.point[1] = 0.6f;
  result.verts.push_back(point);

  point.point[0] = 0.1f;
  point.point[1] = 0.8f;
  result.verts.push_back(point);

  point.point[0] = -0.1f;
  point.point[1] = 0.8f;
  result.verts.push_back(point);

  //Triangle
  point.point[0] = -0.8f;
  point.point[1] = -0.1f;
  result.verts.push_back(point);

  point.point[0] = -0.6f;
  point.point[1] = -0.1f;
  result.verts.push_back(point);

  point.point[0] = -0.7f;
  point.point[1] = 0.1f;
  result.verts.push_back(point);

  //Cross
  point.point[0] = -0.1f;
  point.point[1] = -0.8f;
  result.verts.push_back(point);

  point.point[0] = 0.1f;
  point.point[1] = -0.6f;
  result.verts.push_back(point);

  point.point[0] = -0.1f;
  point.point[1] = -0.6f;
  result.verts.push_back(point);

  point.point[0] = 0.1f;
  point.point[1] = -0.8f;
  result.verts.push_back(point);

  //Plus
  point.point[0] = 0.7f;
  point.point[1] = -0.1f;
  result.verts.push_back(point);

  point.point[0] = 0.8f;
  point.point[1] = 0.0f;
  result.verts.push_back(point);

  point.point[0] = 0.7f;
  point.point[1] = 0.1f;
  result.verts.push_back(point);

  point.point[0] = 0.6f;
  point.point[1] = 0.0f;
  result.verts.push_back(point);

  Renderer::Line line;

  //Square
  line.indices[0] = 0;
  line.indices[1] = 1;
  result.lines.push_back(line);

  line.indices[0] = 1;
  line.indices[1] = 2;
  result.lines.push_back(line);

  line.indices[0] = 2;
  line.indices[1] = 3;
  result.lines.push_back(line);

  line.indices[0] = 3;
  line.indices[1] = 0;
  result.lines.push_back(line);

  //Triangle
  line.indices[0] = 4;
  line.indices[1] = 5;
  result.lines.push_back(line);

  line.indices[0] = 5;
  line.indices[1] = 6;
  result.lines.push_back(line);

  line.indices[0] = 6;
  line.indices[1] = 4;
  result.lines.push_back(line);

  //Cross
  line.indices[0] = 7;
  line.indices[1] = 8;
  result.lines.push_back(line);

  line.indices[0] = 9;
  line.indices[1] = 10;
  result.lines.push_back(line);

  //Plus
  line.indices[0] = 11;
  line.indices[1] = 13;
  result.lines.push_back(line);

  line.indices[0] = 14;
  line.indices[1] = 12;
  result.lines.push_back(line);

  return result;
}

Renderer::LineMesh GenerateLineWheel()
{
  float pi = 3.14159f;

  int n_sides = 5;
  int depth = 5;
  float startRadius = 0.5;
  float endRadius = 0.1f;

  Renderer::LineMesh result;
  Renderer::Line line;
  Renderer::LineVertex point;
  point.point[2] = 0.0f;
  int indexOffset = 0;

  for (int i = 0; i < depth; i++)
  {
    float radius = startRadius + (endRadius - startRadius) * float((depth - 1) - i) / float(depth - 1);

    for (int j = 0; j < n_sides; j++)
    {
      point.point[0] = radius * cos(2.0f * pi * float(j) / float(n_sides));
      point.point[1] = radius * sin(2.0f * pi * float(j) / float(n_sides));

      result.verts.push_back(point);

      line.indices[0] = j + indexOffset;
      line.indices[1] = (j + 1) % n_sides + indexOffset;
      result.lines.push_back(line);
    }

    indexOffset += n_sides;
    n_sides++;
  }

  return result;
}

Renderer::LineMesh GenerateLineSpiral()
{
  float pi = 3.14159f;

  int n_segments = 128;
  float startRadius = 0.1f;
  float endRadius = 0.5f;
  int n_rotations = 3;

  Renderer::LineMesh result;
  Renderer::LineVertex point;
  Renderer::Line line;
  point.point[2] = 0.0f;

  for (int i = 0; i < n_segments; i++)
  {
    float radius = startRadius + (endRadius - startRadius) * float((n_segments - 1) - i) / float(n_segments - 1);
    float angle = 2.0f * pi * n_rotations / n_segments * i;

    point.point[0] = radius * cos(angle);
    point.point[1] = radius * sin(angle);

    result.verts.push_back(point);
  }

  for (int i = 0; i < n_segments - 1; i++)
  {
    line.indices[0] = i;
    line.indices[1] = (i + 1) % n_segments;
    result.lines.push_back(line);
  }

  return result;
}

Renderer::TriangleMesh GenerateFilledShape(int a_nSides)
{
  float pi = 3.14159f;
  float radius = 0.5;

  Renderer::TriangleMesh   result;
  Renderer::TriangleVertex vertex;
  Renderer::Triangle       tri;
  vertex.point[2] = 0.0f;
  vertex.normal[0] = 0.0f;
  vertex.normal[1] = 0.0f;
  vertex.normal[2] = 1.0f;
  vertex.uv[0] = 0.0f;
  vertex.uv[1] = 0.0f;
  tri.indices[0] = 0;

  vertex.point[0] = 0.0f;
  vertex.point[1] = 0.0f;
  result.verts.push_back(vertex);

  for (int i = 0; i < a_nSides; i++)
  {
    float angle = (2.0f * pi) * float(i) / float(a_nSides);

    vertex.point[0] = radius * cos(angle);
    vertex.point[1] = radius * sin(angle);
    result.verts.push_back(vertex);
  }

  for (size_t i = 0; i < a_nSides; i++)
  {
    tri.indices[1] = i + 1;
    tri.indices[2] = (i + 1) % a_nSides + 1;
    result.triangles.push_back(tri);
  }

  return result;
}

Renderer::TriangleMesh GenerateTriangleBox()
{
  Renderer::TriangleMesh   result;
  Renderer::TriangleVertex vertex;

  vertex.point[2] = 0.0f;
  vertex.normal[0] = 0.0f;
  vertex.normal[1] = 0.0f;
  vertex.normal[2] = 1.0f;

  ///

  vertex.point[0] = -0.5f;
  vertex.point[1] = -0.5f;
  vertex.uv[0] = 0.0f;
  vertex.uv[1] = 1.0f;
  result.verts.push_back(vertex);

  vertex.point[0] = 0.5f;
  vertex.point[1] = -0.5f;
  vertex.uv[0] = 1.0f;
  vertex.uv[1] = 1.0f;
  result.verts.push_back(vertex);

  vertex.point[0] = 0.5f;
  vertex.point[1] = 0.5f;
  vertex.uv[0] = 1.0f;
  vertex.uv[1] = 0.0f;
  result.verts.push_back(vertex);

  vertex.point[0] = -0.5f;
  vertex.point[1] = 0.5f;
  vertex.uv[0] = 0.0f;
  vertex.uv[1] = 0.0f;
  result.verts.push_back(vertex);

  Renderer::Triangle tri;

  tri.indices[0] = 3;
  tri.indices[1] = 1;
  tri.indices[2] = 0;
  result.triangles.push_back(tri);

  tri.indices[0] = 3;
  tri.indices[1] = 1;
  tri.indices[2] = 2;
  result.triangles.push_back(tri);

  return result;
}