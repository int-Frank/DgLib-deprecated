#include <iostream>
#include "Mod_Context_Logger.h"
#include "Mod_Context.h"

namespace Context
{
  void Init(void(*Log)(std::string const &, int))
  {
    Logger::Init(Log);
  }

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