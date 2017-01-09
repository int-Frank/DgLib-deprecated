//! @file DgGeometryTypes.h

#ifndef DGGEOMETRYTYPES_H
#define DGGEOMETRYTYPES_H

namespace Dg
{
  struct Error_geometry_type_not_specialized_for_this_type
  {
  };

  template<typename Real, int R>
  struct GeometryTypes
  {
    typedef Error_geometry_type_not_specialized_for_this_type vec;
    typedef Error_geometry_type_not_specialized_for_this_type matrix;
    typedef Error_geometry_type_not_specialized_for_this_type vqs;
  };
}

#endif