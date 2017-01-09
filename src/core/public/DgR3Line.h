//! @file DgR3Line.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGR3LINE_H
#define DGR3LINE_H

#include "impl/DgLine_general.h"
#include "DgR3GeometryTypes.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Line = Dg::impl::Line_general<Real, 3>;
  }
}
#endif