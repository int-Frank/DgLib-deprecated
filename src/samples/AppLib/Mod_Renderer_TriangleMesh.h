#ifndef MOD_RENDERER_TRIANGLEMESH_H
#define MOD_RENDERER_TRIANGLEMESH_H

#include <vector>
#include "DgMatrix.h"

namespace Renderer
{
  typedef Dg::Matrix<1, 3, float>   vec3;
  typedef Dg::Matrix<1, 2, float>   vec2;

  struct TriangleVertex
  {
    vec3 point;
    vec3 normal;
    vec2 UV;
  };

  struct Triangle
  {
    int indices[3];
  };

  struct TriangleMesh
  {
    std::vector<TriangleVertex>   verts;
    std::vector<Triangle> triangles;
  };
}


#endif