//! @file DgQueryLineSegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESEGMENTLINE_H
#define DGQUERYLINESEGMENTLINE_H

#include "DgDCPQuery.h"
#include "..\DgLine.h"
#include "..\DgLineSegment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Line segment, Line
  template <typename Real>
  class DCPQuery<Real, LineSegment<Real>, Line<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Distance between line segment and line
      Real distance;

      //! Squared distance between line segment and line
      Real sqDistance;

      //! Distance from the line origin to closest point to the line segment
      Real ul;

      //! Distance from the line segment origin to closest point to the line
      Real uls;

      //! Closest point on line to the line segment
      Vector4<Real> cpl;

      //! Closest point on line segment to the line
      Vector4<Real> cpls;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Line segment and line are parallel.
      int code;
    };

    //! Perform query.
    Result operator()(LineSegment<Real> const &, Line<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using DCPLineSegmentLine = DCPQuery<Real, LineSegment<Real>, Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, LineSegment<Real>, Line<Real>>::Result
    DCPQuery<Real, LineSegment<Real>, Line<Real>>::operator()
    (LineSegment<Real> const & a_ls, Line<Real> const & a_line)
  {
    Result result;

    Vector4<Real> ols(a_ls.Origin());
    Vector4<Real> ol(a_line.Origin());
    Vector4<Real> dls(a_ls.Direction());
    Vector4<Real> dl(a_line.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(ols - ol);
    Real a = dls.Dot(dls);
    Real b = dls.Dot(dl);
    Real c = dls.Dot(w0);
    Real d = dl.Dot(w0);
    Real denom = a - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if (Dg::IsZero(denom))
    {
      result.uls = static_cast<Real>(0.0);
      result.ul = d;
      result.code = 1;
    }
    else
    {
      result.code = 0;

      // clamp result.uls within [0,1]
      Real sn = b*d - c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0.0))
      {
        result.uls = static_cast<Real>(0.0);
        result.ul = d;
      }
      // clamp result.uls to 1
      else if (sn > denom)
      {
        result.uls = static_cast<Real>(1.0);
        result.ul = (d + b);
      }
      else
      {
        result.uls = sn / denom;
        result.ul = (a*d - b*c) / denom;
      }
    }

    result.cpls = ols +result.uls*dls;
    result.cpl = ol + result.ul*dl;
    result.sqDistance = SquaredDistance(result.cpls, result.cpl);
    result.distance = sqrt(result.sqDistance);
    return result;
  } //End: DCPQuery::operator()
}

#endif