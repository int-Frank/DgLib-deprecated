//! @file DgQueryPointLineSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTLINESEGMENT_H
#define DGQUERYPOINTLINESEGMENT_H

#include "DgCPQuery.h"
#include "..\DgLineSegment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, LineSegment.
  template <typename Real>
  class CPQuery<Real, Vector4<Real>, LineSegment<Real>>
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
    Result operator()(Vector4<Real> const &, LineSegment<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointLineSegment = CPQuery<Real, Vector4<Real>, LineSegment<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, Vector4<Real>, LineSegment<Real>>::Result
    CPQuery<Real, Vector4<Real>, LineSegment<Real>>::operator()
    (Vector4<Real> const & a_point, LineSegment<Real> const & a_line)
  {
    Result result;
    Vector4<Real> w = a_point - a_line.Origin();

    Real proj = w.Dot(a_line.Direction());

    if (proj <= static_cast<Real>(0.0))
    {
      result.u = static_cast<Real>(0.0);
    }
    else
    {
      Real vsq = a_line.LengthSquared();

      if (proj >= vsq)
      {
        result.u = static_cast<Real>(1.0);
      }
      else
      {
        result.u = (proj / vsq);
      }
    }

    result.cp = a_line.Origin() + result.u * a_line.Direction();
    return result;
  }
} //End: CPQuery::operator()

#endif