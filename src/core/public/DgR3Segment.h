//! @file DgR3Segment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment_general

#ifndef DGR3SEGMENT_H
#define DGR3SEGMENT_H

#include "impl/DgSegment_general.h"
#include "DgR3GeometryTypes.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Segment = Dg::impl::Segment_general<Real, 3>;
  }
}
#endif