#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"

class Renderer
{
public:
  Renderer(){}

  void Begin();
  void End();

  void DrawSegment(Segment const &, Vector const & color);
  void DrawPolygon(Polygon const &, Vector const & color);

private:
};

#endif
