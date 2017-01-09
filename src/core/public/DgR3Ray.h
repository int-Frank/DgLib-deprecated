//! @file DgR3Ray.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray

#ifndef DGR3RAY_H
#define DGR3RAY_H

#include "impl/DgRay_general.h"
#include "DgGeometryTypesR3.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Ray = Dg::impl::Ray_general<Real, 3>;
  }
}
#endif