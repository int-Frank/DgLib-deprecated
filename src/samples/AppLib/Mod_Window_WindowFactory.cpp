
#include "Mod_Window_Window.h"
#include "Mod_Window_Window_ImGui.h"

namespace Window
{
  Window * WindowFactory::GetNewWindow(InitData const & a_data)
  {
    if (a_data.name.empty())
    {
      return nullptr;
    }
    return new Window_ImGui(a_data);
  }
}