#ifndef MOD_RENDERER_LINEMESH_H
#define MOD_RENDERER_LINEMESH_H

#include <stdint.h>
#include <vector>
#include "DgR3Vector.h"

namespace Renderer
{
  typedef Dg::Matrix<1, 3, float>   vec3;

  struct LineVertex
  {
    vec3 point;
  };

  struct Line
  {
    int indices[2];
  };

  struct LineMesh
  {
    std::vector<LineVertex>  verts;
    std::vector<Line>        lines;
  };
}

#endif