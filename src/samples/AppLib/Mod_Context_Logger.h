#ifndef MOD_CONTEXT_LOGGER_H
#define MOD_CONTEXT_LOGGER_H

#include <string>

namespace Context
{
  namespace Logger
  {
    void Init(void(*Log)(std::string const &, int));
    void Log(std::string const &, int);
  }
}

#endif