#ifndef TYPES_H
#define TYPES_H

#include <string>
#include "DgR2Matrix.h"
#include "DgR2Segment.h"
#include "DgR2Polygon.h"
#include "DgR2AABB.h"
#include "game_tools/DgGTBSPTree.h"

typedef Dg::R2::Vector<float>       Vector;
typedef Dg::R2::Matrix<float>       Matrix;
typedef Dg::R2::Segment<float>      Segment;
typedef Dg::R2::Polygon<float>      Polygon;
typedef Dg::R2::AABB<float>         AABB;
typedef Dg::GT::BSPTree<float, int> BSPTree;

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

class AppState
{
public:

  AppState()
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