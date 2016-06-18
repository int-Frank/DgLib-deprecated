//! @file DgQueryLineSegmentRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESEGMENTRAY_H
#define DGQUERYLINESEGMENTRAY_H

#include "DgDCPQuery.h"
#include "..\DgRay.h"
#include "..\DgLineSegment.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Line Segment, Ray
  template <typename Real>
  class DCPQuery<Real, LineSegment<Real>, Ray<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Distance between line segment and ray
      Real distance;

      //! Squared distance between line segment and ray
      Real sqDistance;

      //! Distance from the ray origin to closest point to the line segment
      Real ur;

      //! Distance from the line segment origin to closest point to the ray
      Real uls;

      //! Closest point on ray to the line segment
      Vector4<Real> cpr;

      //! Closest point on line segment to the ray
      Vector4<Real> cpls;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Line segment and ray are parallel and overlap - infinite closest points.
      int code;
    };

    //! Perform query.
    Result operator()(LineSegment<Real> const &, Ray<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using DCPLineSegmentRay = DCPQuery<Real, LineSegment<Real>, Ray<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, LineSegment<Real>, Ray<Real>>::Result
    DCPQuery<Real, LineSegment<Real>, Ray<Real>>::operator()
    (LineSegment<Real> const & a_ls, Ray<Real> const & a_ray)
  {
    Result result;
    result.code = 0;

    Vector4<Real> ols(a_ls.Origin());
    Vector4<Real> or (a_ray.Origin());
    Vector4<Real> dls(a_ls.Direction());
    Vector4<Real> dr(a_ray.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(ols - or );
    Real a = dls.Dot(dls);
    Real b = dls.Dot(dr);
    Real c = dls.Dot(w0);
    Real d = dr.Dot(w0);
    Real denom = a - b*b;

    Real sn, sd, tn, td;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      // clamp result.uls to 0
      sd = td = static_cast<Real>(1.0);
      sn = static_cast<Real>(0.0);
      tn = d;

      //Does the live segment overlap the ray?
      Vector4<Real> w1((ols + dls) - or );
      if (!(d < static_cast<Real>(0.0) &&
          w1.Dot(dr) < static_cast<Real>(0.0)))
      {
        result.code = 1;
      }

    }
    else
    {
      // clamp result.uls within [0,1]
      sd = td = denom;
      sn = b*d - c;
      tn = a*d - b*c;

      // clamp result.uls to 0
      if (sn < static_cast<Real>(0.0))
      {
        sn = static_cast<Real>(0.0);
        tn = d;
        td = static_cast<Real>(1.0);
      }
      // clamp result.uls to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = d + b;
        td = static_cast<Real>(1.0);
      }
    }

    // clamp result.ur within [0,+inf]
    // clamp result.ur to 0
    if (tn < static_cast<Real>(0.0))
    {
      result.ur = static_cast<Real>(0.0);
      // clamp result.uls to 0
      if (-c < static_cast<Real>(0.0))
      {
        result.uls = static_cast<Real>(0.0);
      }
      // clamp result.uls to 1
      else if (-c > a)
      {
        result.uls = static_cast<Real>(1.0);
      }
      else
      {
        result.uls = -c / a;
      }
    }
    else
    {
      result.ur = tn / td;
      result.uls = sn / sd;
    }

    result.cpls = ols + result.uls*dls;
    result.cpr = or + result.ur*dr;
    result.sqDistance = SquaredDistance(result.cpls, result.cpr);
    result.distance = sqrt(result.sqDistance);
    return result;
  } //End: DCPQuery::operator()
}

#endif