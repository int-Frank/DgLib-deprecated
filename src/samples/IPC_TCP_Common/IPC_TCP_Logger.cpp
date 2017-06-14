#include <iostream>
#include "IPC_TCP_Logger.h"

namespace IPC
{
  namespace TCP
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
}