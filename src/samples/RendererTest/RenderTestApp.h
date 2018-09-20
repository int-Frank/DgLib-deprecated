#ifndef RENDERTESTAPP_H
#define RENDERTESTAPP_H

#include <vector>

#include "DgApp.h"
#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "Mod_Context_Line.h"
#include "Mod_Context_Triangle.h"
#include "Mod_Window.h"

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
  void WindowSizeCallback(int w, int h);

private:

  struct ScreenObject
  {
    Context::ObjectHandle  handle;
    mat4                   transform;
    bool                   show;
  };

  Context::ContextLine      m_contextLine;
  Context::ContextTriangle  m_contextTriangle;
  Window::Window *          m_pWindow;
  Window::Window *          m_pViewport;
  std::vector<ScreenObject> m_lineObjects;
  std::vector<ScreenObject> m_triangleObjects;
};

#endif