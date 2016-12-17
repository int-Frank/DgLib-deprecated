//! @file DgQuerySegmentSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYSEGMENTSEGMENT_H
#define DGQUERYSEGMENTSEGMENT_H

#include "DgCPQuery.h"
#include "..\DgSegment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Line Segment, Line Segment
  template <typename Real>
  class CPQuery<Real, Segment<Real>, Segment<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Fraction along segment 0 to closest point to the segment 1
      Real u0;

      //! Fraction along segment 1 to closest point to the segment 0
      Real u1;

      //! Closest point on segment 0 to segment 1
      Vector4<Real> cp0;

      //! Closest point on segment 1 to segment 0
      Vector4<Real> cp1;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Line segments are parallel and overlap - infinite closest points.
      int code;
    };

    //! Perform query.
    Result operator()(Segment<Real> const &, Segment<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using CPSegmentSegment = CPQuery<Real, Segment<Real>, Segment<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, Segment<Real>, Segment<Real>>::Result
    CPQuery<Real, Segment<Real>, Segment<Real>>::operator()
    (Segment<Real> const & a_seg0, Segment<Real> const & a_seg1)
  {
    Result result;
    result.code = 0;

    Vector4<Real> o0(a_seg0.Origin());
    Vector4<Real> o1(a_seg1.Origin());
    Vector4<Real> d0(a_seg0.Direction());
    Vector4<Real> d1(a_seg1.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d0);
    Real b = d0.Dot(d1);
    Real c = d1.Dot(d1);
    Real d = d0.Dot(w0);
    Real e = d1.Dot(w0);
    Real denom = a*c - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      // clamp result.u0 to 0
      sd = td = c;
      sn = static_cast<Real>(0.0);
      tn = e;

      //Do the line segments overlap?
      Vector4<Real> w1((o0 + d0) - o1);
      Vector4<Real> w2(o0 - (o1 + d1));
      Vector4<Real> w3((o0 + d0) - (o1+ d1));
      bool bse = (e < static_cast<Real>(0.0));
      if (!(
            bse == (w1.Dot(d1) < static_cast<Real>(0.0)) &&
            bse == (w2.Dot(d1) < static_cast<Real>(0.0)) &&
            bse == (w3.Dot(d1) < static_cast<Real>(0.0))
           ))
      {
        result.code = 1;
      }
    }
    else
    {
      // clamp result.u0 within [0,1]
      sd = td = denom;
      sn = b*e - c*d;
      tn = a*e - b*d;

      // clamp result.u0 to 0
      if (sn < static_cast<Real>(0.0))
      {
        sn = static_cast<Real>(0.0);
        tn = e;
        td = c;
      }
      // clamp result.u0 to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = e + b;
        td = c;
      }
    }

    // clamp result.u1 within [0,1]
    // clamp result.u1 to 0
    if (tn < static_cast<Real>(0.0))
    {
      result.u1 = static_cast<Real>(0.0);
      // clamp result.u0 to 0
      if (-d < static_cast<Real>(0.0))
      {
        result.u0 = static_cast<Real>(0.0);
      }
      // clamp result.u0 to 1
      else if (-d > a)
      {
        result.u0 = static_cast<Real>(1.0);
      }
      else
      {
        result.u0 = -d / a;
      }
    }
    // clamp result.u1 to 1
    else if (tn > td)
    {
      result.u1 = static_cast<Real>(1.0);
      // clamp result.u0 to 0
      if ((-d + b) < static_cast<Real>(0.0))
      {
        result.u0 = static_cast<Real>(0.0);
      }
      // clamp result.u0 to 1
      else if ((-d + b) > a)
      {
        result.u0 = static_cast<Real>(1.0);
      }
      else
      {
        result.u0 = (-d + b) / a;
      }
    }
    else
    {
      result.u1 = tn / td;
      result.u0 = sn / sd;
    }

    result.cp0 = o0 + result.u0*d0;
    result.cp1 = o1 + result.u1*d1;
    return result;
  } //End: CPQuery::operator()
}

#endif