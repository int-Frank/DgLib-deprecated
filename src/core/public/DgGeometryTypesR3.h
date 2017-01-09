//! @file DgGeometryTypesR3.h

#ifndef DGGEOMETRYTYPESR3_H
#define DGGEOMETRYTYPESR3_H

#include "DgGeometryTypes.h"
#include "DgR3Vector4.h"
#include "DgR3Matrix44.h"
#include "DgR3VQS.h"

namespace Dg
{
  template<typename Real>
  struct GeometryTypes<Real, 3>
  {
    typedef R3::Vector4<Real>   vec;
    typedef R3::Matrix44<Real>  matrix;
    typedef R3::VQS<Real>       vqs;
  };
}

#endif