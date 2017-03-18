
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "DgMakeGrid.h"
#include "Types.h"
#include "UI.h"

Renderer::Renderer()
{
  m_T.Identity();
}

void Renderer::SetMatrix(Matrix const & a_T)
{
  m_T = a_T;
}

void Renderer::Begin()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}


void Renderer::End()
{
}

void Renderer::DrawSegment(Segment const & a_seg, Vector const & a_color)
{
  Vector p0 = a_seg.GetP0() * m_T;
  Vector p1 = a_seg.GetP1() * m_T;

  glBegin(GL_LINES);
  glColor4f(a_color[0], a_color[2], a_color[2], a_color[3]);
  glVertex2f(p0.x(), p0.y());
  glVertex2f(p1.x(), p1.y());
  glEnd();
}

void Renderer::DrawPolygon(Polygon const & a_poly, Vector const & a_color)
{
  glBegin(GL_POLYGON);
  glColor4f(a_color[0], a_color[2], a_color[2], a_color[3]);
  auto it = a_poly.chead();
  for (size_t i = 0; i < a_poly.size(); ++i, ++it)
  {
    Vector p = *it * m_T;
    glVertex2f(p.x(), p.y());
  }
  glEnd();
}