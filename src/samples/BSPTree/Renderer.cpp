
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "DgMakeGrid.h"
#include "Types.h"
#include "UI.h"

Renderer::Renderer()
{
}

void Renderer::Begin()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}


void Renderer::End()
{
}

void Renderer::DrawSegment(Segment const & a_seg, Color const & a_color)
{
}

void Renderer::DrawPolygon(Polygon const & a_poly, Color const & a_color)
{
}