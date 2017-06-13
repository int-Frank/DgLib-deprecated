
#include <iostream>
#include "IPC_TCP_Mediator.h"

namespace IPC
{
  namespace TCP
  {
    namespace impl
    {
      void LogDefault(std::string const & a_str, int a_logLevel)
      {
        std::cout << a_str << ": " << a_logLevel << "\n";
      }

      bool ShouldStopDefault()
      {
        return false;
      }
    }

    void(*Mediator::Log)(std::string const &, int) = &impl::LogDefault;
    bool(*Mediator::ShouldStop)() = &impl::ShouldStopDefault;
  }
}