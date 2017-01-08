//! @file DgQueryCommon.h

#ifndef DGQUERYCOMMON_H
#define DGQUERYCOMMON_H

namespace Dg
{
  enum QueryCode
  {
    Fail = 0,
    Success,
    Parallel,
    Overlapping,
    Intersecting,
    NotIntersecting,
    CompletelyInside,
    CompletelyOutside
  };
}

#endif