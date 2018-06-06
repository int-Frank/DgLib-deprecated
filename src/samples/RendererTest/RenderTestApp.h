#ifndef RENDERTESTAPP_H
#define RENDERTESTAPP_H

#include <vector>

#include "DgApp.h"
#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "Mod_Renderer_Renderer.h"
#include "Mod_Renderer_Window.h"

class RenderTestApp : public DgApp
{
  typedef Dg::R3::Vector<float> vec4;
  typedef Dg::R3::Matrix<float> mat4;

public:

  RenderTestApp();
  ~RenderTestApp();

protected:

  void BuildUI();
  void DoFrame(double dt);
  void WindowSizeCallback(int h, int w);
  void SetTransforms();

private:

  struct ScreenObject
  {
    Renderer::ObjectHandle  handle;
    mat4          transform;
    vec4          color;
  };

  static int const          s_nObjects = 9;
  bool                      m_showObject[s_nObjects];
  Renderer::Renderer *      m_pRender;
  Renderer::Window *        m_pWindow;
  std::vector<ScreenObject> m_lineObjects;
  std::vector<ScreenObject> m_triangleObjects;
};

#endif