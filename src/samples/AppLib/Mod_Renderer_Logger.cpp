#include <iostream>
#include "Mod_Renderer_Logger.h"

namespace Renderer
{
  namespace Logger
  {
    static void LogDefault(std::string const & a_str, int a_logLevel)
    {
      std::cout << a_str << ": " << a_logLevel << "\n";
    }

    static void(*g_Log)(std::string const &, int) = LogDefault;

    void Init(void(*Log)(std::string const &, int))
    {
      g_Log = Log;
    }

    void Log(std::string const & a_message, int a_lvl)
    {
      g_Log(a_message, a_lvl);
    }
  }
}