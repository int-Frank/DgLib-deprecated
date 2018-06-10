
#include "Mod_Window.h"
#include "Mod_Window_Window_ImGui.h"
#include "Mod_Window_Window_Viewport.h"

namespace Window
{
  Window * WindowFactory::GetNewWindow(InitData const & a_data)
  {
    if (a_data.isImGuiWindow)
    {
      return new Window_ImGui(a_data);
    }
    return new Window_Viewport(a_data);
  }
}