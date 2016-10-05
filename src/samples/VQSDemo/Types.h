#ifndef TYPES_H
#define TYPES_H

#include <string>

#include "DgMatrix44.h"

struct AppInfo
{
  char title[128];
  int windowWidth;
  int windowHeight;
  int majorVersion;
  int minorVersion;
  int samples;
  union
  {
    struct
    {
      unsigned int    fullscreen : 1;
      unsigned int    vsync : 1;
      unsigned int    debug : 1;
    };
    unsigned int        all;
  } flags;
};

class AppData
{
public:

  AppData()
    : dirty(false)
  {}

  bool                  dirty;
  std::string           projName;
};
#endif