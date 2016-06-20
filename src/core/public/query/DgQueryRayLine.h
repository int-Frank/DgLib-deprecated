//! @file DgQueryRayLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYRAYLINE_H
#define DGQUERYRAYLINE_H

#include "DgDCPQuery.h"
#include "..\DgLine.h"
#include "..\DgRay.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Ray, Line
  template <typename Real>
  class DCPQuery<Real, Ray<Real>, Line<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Distance between ray and line
      Real distance;

      //! Squared distance between ray and line
      Real sqDistance;

      //! Distance from the line origin to closest point to the ray
      Real ul;

      //! Distance from the ray origin to closest point to the line
      Real ur;

      //! Closest point on line to the ray
      Vector4<Real> cpl;

      //! Closest point on ray to the line
      Vector4<Real> cpr;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Ray and line are parallel. Closest points are based off the ray origin.
      int code;
    };

    //! Perform query.
    Result operator()(Ray<Real> const &, Line<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using DCPRayLine = DCPQuery<Real, Ray<Real>, Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, Ray<Real>, Line<Real>>::Result
    DCPQuery<Real, Ray<Real>, Line<Real>>::operator()
    (Ray<Real> const & a_ray, Line<Real> const & a_line)
  {
    Vector4<Real> or (a_ray.Origin());
    Vector4<Real> ol(a_line.Origin());
    Vector4<Real> dr(a_ray.Direction());
    Vector4<Real> dl(a_line.Direction());

    //compute intermediate parameters
    Vector4<Real> diff(or -ol);
    Real a = dr.Dot(dl);
    Real b = dl.Dot(diff);
    Real c = static_cast<Real>(1.0) - a * a;

    Result result;
    result.code = 0;

    //if c is zero, try finding closest point on the ray to the line origin
    if (Dg::IsZero(c))
    {
      //compute closest points
      result.ur = static_cast<Real>(0.0);
      result.ul = b;
      result.code = 1;
    }
    else
    {
      // clamp ur within [0,+inf]
      Real d = dr.Dot(diff);
      Real sn = a * b - d;

      // clamp ur to 0
      if (sn < static_cast<Real>(0.0))
      {
        result.ur = static_cast<Real>(0.0);
        result.ul = b;
      }
      else
      {
        Real denom = static_cast<Real>(1.0) / c;
        result.ur = sn * denom;
        result.ul = (b - a*d) * denom;
      }

      result.code = 0;
    }

    result.cpr = or + result.ur*dr;
    result.cpl = ol + result.ul*dl;
    result.sqDistance = SquaredDistance(result.cpr, result.cpl);
    result.distance = sqrt(result.sqDistance);
    return result;
  } //End: DCPQuery::operator()
}

#endif