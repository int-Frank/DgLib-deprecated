//! @file DgR3QuerySegmentSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTLINE_H
#define DGR3QUERYSEGMENTLINE_H

#include "..\impl\DgQuerySegmentSphere.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using CPSegmentSphere = impl::TIQuery<Real, 3, R3::Segment<3>, Sphere<Real>>;
  }
}

#endif