
#include <iostream>
#include "IPC_TCP_Mediator.h"

namespace IPC
{
  namespace TCP
  {
    static void LogDefault(std::string const & a_str, int a_logLevel)
    {
      std::cout << a_str << ": " << a_logLevel << "\n";
    }

    static bool ShouldStopDefault()
    {
      return false;
    }

    void(*Mediator::Log)(std::string const &, int) = &LogDefault;
    bool(*Mediator::ShouldStop)() = &ShouldStopDefault;
  }
}