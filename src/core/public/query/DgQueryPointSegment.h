//! @file DgQueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTSEGMENT_H
#define DGQUERYPOINTSEGMENT_H

#include "DgCPQuery.h"
#include "..\DgSegment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, Segment.
  template <typename Real>
  class CPQuery<Real, Vector4<Real>, Segment<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance from line segment origin to closest point to point
      Real u;

      //! Closest point on the line segment to the point.
      Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(Vector4<Real> const &, Segment<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointSegment = CPQuery<Real, Vector4<Real>, Segment<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, Vector4<Real>, Segment<Real>>::Result
    CPQuery<Real, Vector4<Real>, Segment<Real>>::operator()
    (Vector4<Real> const & a_point, Segment<Real> const & a_seg)
  {
    Result result;
    Vector4<Real> w = a_point - a_seg.Origin();

    Real proj = w.Dot(a_seg.Direction());

    if (proj <= static_cast<Real>(0.0))
    {
      result.u = static_cast<Real>(0.0);
    }
    else
    {
      Real vsq = a_seg.LengthSquared();

      if (proj >= vsq)
      {
        result.u = static_cast<Real>(1.0);
      }
      else
      {
        result.u = (proj / vsq);
      }
    }

    result.cp = a_seg.Origin() + result.u * a_seg.Direction();
    return result;
  }
} //End: CPQuery::operator()

#endif