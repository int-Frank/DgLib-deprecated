#ifndef TYPES_H
#define TYPES_H

#include <string>

enum Flags
{
  E_Visualize_Leaves = 0,
  E_Visualize_Node = 1,
};

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
    : visType(E_Visualize_Node)
    , objectType(0)
    , objectRadius(1.0f)
  {}

  float               objectRadius;
  int                 objectType;
  int                 visType;
  std::string         projName;
};
#endif