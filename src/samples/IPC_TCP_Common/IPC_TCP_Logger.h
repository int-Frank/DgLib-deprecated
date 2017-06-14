#ifndef IPC_TCP_LOGGER_H
#define IPC_TCP_LOGGER_H

#include <string>

namespace IPC
{
  namespace TCP
  {
    namespace Logger
    {
      void Init(void(*Log)(std::string const &, int));
      void Log(std::string const &, int);
    }
  }
}
#endif