//! @file DgR2Line.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGR2LINE_H
#define DGR2LINE_H

#include "impl/DgLine_general.h"
#include "DgR2GeometryTypes.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Line = Dg::impl::Line_general<Real, 2>;
  }
}
#endif