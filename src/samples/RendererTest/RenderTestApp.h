#ifndef RENDERTESTAPP_H
#define RENDERTESTAPP_H

#include <vector>

#include "DgApp.h"
#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "Mod_Renderer_Renderer.h"
#include "Mod_Window_Window.h"

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
    Renderer::ObjectHandle  handle;
    mat4                    transform;
    vec4                    color;
    bool                    show;
  };

  Renderer::Renderer *      m_pRender;
  Window::Window *          m_pWindow;
  Window::Window *          m_pViewport;
  std::vector<ScreenObject> m_lineObjects;
  std::vector<ScreenObject> m_triangleObjects;
};

#endif