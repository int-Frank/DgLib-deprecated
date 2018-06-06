#ifndef MOD_RENDERER_WINDOW_IMGUI
#define MOD_RENDERER_WINDOW_IMGUI

#include <string>

#include <GL/glew.h>

#include "DgR3Vector.h"
#include "Mod_Renderer_Window.h"

namespace Renderer
{
  class Window_ImGui : public Window
  {
    typedef Dg::R3::Vector<float> vec4;

  public:

    Window_ImGui(InitData const &);
    ~Window_ImGui();

    void Update();

  private:

    std::string m_name;

    float const m_marginX;
    float const m_marginY;
  };
}

#endif