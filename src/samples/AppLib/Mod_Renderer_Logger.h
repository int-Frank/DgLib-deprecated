#ifndef MOD_RENDERER_LOGGER_H
#define MOD_RENDERER_LOGGER_H

#include <string>

namespace Renderer
{
  namespace Logger
  {
    void Init(void(*Log)(std::string const &, int));
    void Log(std::string const &, int);
  }
}

#endif