#ifndef MOD_WINDOW_WINDOW_IMGUI
#define MOD_WINDOW_WINDOW_IMGUI

#include <string>

#include <GL/glew.h>

#include "DgR3Vector.h"
#include "Mod_Window.h"

namespace Window
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

    int         m_flags;

    float const m_marginX;
    float const m_marginY;
  };
}

#endif