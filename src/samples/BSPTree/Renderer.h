#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"

struct Color
{
  float r, g, b, a;
};

class Renderer
{
public:
  Renderer();

  void Begin();
  void End();

  void DrawSegment(Segment const &, Color const & color);
  void DrawPolygon(Polygon const &, Color const & color);

};

#endif
