#ifndef TYPES_H
#define TYPES_H

#include <string>

#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "DgR3Segment.h"
#include "DgR3Ray.h"
#include "DgR3Triangle.h"
#include "DgR3Sphere.h"

typedef Dg::R3::Vector<float>    vec4;
typedef Dg::R3::Segment<float>   seg;
typedef Dg::R3::Ray<float>       ray;
typedef Dg::R3::Triangle<float>  triangle;
typedef Dg::R3::Sphere<float>    sphere;
typedef Dg::R3::Matrix<float>    mat44;

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
    : source(vec4::Origin())
    , power(1.f)
    , retardingForce(1.f)
    , explode(false)
  {}

  std::string   projName;
  vec4          source;
  float         power;
  float         retardingForce;
  bool          explode;
};
#endif