//! @file DgQuerySegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESEGMENTLINE_H
#define DGQUERYLINESEGMENTLINE_H

#include "DgCPQuery.h"
#include "..\DgR3Line.h"
#include "..\DgR3Segment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: R3::Line segment, R3::Line
  template <typename Real>
  class CPQuery<Real, R3::Segment<Real>, R3::Line<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Distance from the line origin to closest point to the line segment
      Real ul;

      //! Distance from the line segment origin to closest point to the line
      Real us;

      //! Closest point on line to the line segment
      R3::Vector4<Real> cpl;

      //! Closest point on line segment to the line
      R3::Vector4<Real> cps;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: R3::Line segment and line are parallel.
      int code;
    };

    //! Perform query.
    Result operator()(R3::Segment<Real> const &, R3::Line<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using CPSegmentLine = CPQuery<Real, R3::Segment<Real>, R3::Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, R3::Segment<Real>, R3::Line<Real>>::Result
    CPQuery<Real, R3::Segment<Real>, R3::Line<Real>>::operator()
    (R3::Segment<Real> const & a_seg, R3::Line<Real> const & a_line)
  {
    Result result;

    R3::Vector4<Real> os(a_seg.Origin());
    R3::Vector4<Real> ol(a_line.Origin());
    R3::Vector4<Real> ds(a_seg.Direction());
    R3::Vector4<Real> dl(a_line.Direction());

    //compute intermediate parameters
    R3::Vector4<Real> w0(os - ol);
    Real a = ds.Dot(ds);
    Real b = ds.Dot(dl);
    Real c = ds.Dot(w0);
    Real d = dl.Dot(w0);
    Real denom = a - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if (Dg::IsZero(denom))
    {
      result.us = static_cast<Real>(0.0);
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
        result.us = static_cast<Real>(0.0);
        result.ul = d;
      }
      // clamp result.uls to 1
      else if (sn > denom)
      {
        result.us = static_cast<Real>(1.0);
        result.ul = (d + b);
      }
      else
      {
        result.us = sn / denom;
        result.ul = (a*d - b*c) / denom;
      }
    }

    result.cps = os +result.us*ds;
    result.cpl = ol + result.ul*dl;
    return result;
  } //End: CPQuery::operator()
}

#endif