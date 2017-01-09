//! @file DgR3GeometryTypes.h

#ifndef DGGEOMETRYTYPESR3_H
#define DGGEOMETRYTYPESR3_H

#include "DgGeometryTypes.h"
#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "DgR3VQS.h"

namespace Dg
{
  template<typename Real>
  struct GeometryTypes<Real, 3>
  {
    typedef R3::Vector<Real>   vec;
    typedef R3::Matrix<Real>  matrix;
    typedef R3::VQS<Real>       vqs;
  };
}

#endif