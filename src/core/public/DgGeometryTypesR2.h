//! @file DgGeometryTypesR2.h

#ifndef DGGEOMETRYTYPESR2_H
#define DGGEOMETRYTYPESR2_H

#include "DgGeometryTypes.h"
#include "DgR2Vector.h"
#include "DgR2Matrix.h"

namespace Dg
{
  template<typename Real>
  struct GeometryTypes<Real, 2>
  {
    typedef R2::Vector<Real>    vec;
    typedef R2::Matrix<Real>  matrix;
    typedef Error_geometry_type_not_specialized_for_this_type vqs;
  };
}

#endif