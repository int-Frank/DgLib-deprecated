#ifndef IPC_TCP_MEDIATOR_H
#define IPC_TCP_MEDIATOR_H

#include <string>

namespace IPC
{
  namespace TCP
  {
    class Mediator
    {
    public:

      static void(*Log)(std::string const &, int);
      static bool(*ShouldStop)();
    };
  }
}
#endif