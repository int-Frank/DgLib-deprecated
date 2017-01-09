//! @file DgR2Ray.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray

#ifndef DGR2RAY_H
#define DGR2RAY_H

#include "impl/DgRay_general.h"
#include "DgGeometryTypesR2.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Ray = Dg::impl::Ray_general<Real, 2>;
  }
}
#endif