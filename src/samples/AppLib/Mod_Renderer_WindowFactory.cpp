
#include "Mod_Renderer_Window.h"
#include "Mod_Renderer_Window_ImGui.h"

namespace Renderer
{
  Window * WindowFactory::GetNewWindow(Window::InitData const & a_data)
  {
    if (a_data.name.empty())
    {
      return nullptr;
    }
    return new Window_ImGui(a_data);
  }
}