#ifndef TYPES_H
#define TYPES_H

#include <string>

#include "DgR3Vector.h"
#include "DgR3Matrix.h"

typedef Dg::R3::Vector<float>   vec4;
typedef Dg::R3::Matrix<float>   mat44;

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

  std::string           projName;
};
#endif