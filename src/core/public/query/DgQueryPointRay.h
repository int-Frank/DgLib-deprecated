//! @file DgQueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTRAY_H
#define DGQUERYPOINTRAY_H

#include "DgDCPQuery.h"
#include "..\DgRay.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, Ray.
  template <typename Real>
  class DCPQuery<Real, Vector4<Real>, Ray<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance between point and ray
      Real distance;

      //! Squared distance between point and ray
      Real sqDistance;

      //! Distance from ray origin to closest point to point
      Real u;

      //! Closest point on the ray to the point.
      Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(Vector4<Real> const &, Ray<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using DCPPointRay = DCPQuery<Real, Vector4<Real>, Ray<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, Vector4<Real>, Ray<Real>>::Result
    DCPQuery<Real, Vector4<Real>, Ray<Real>>::operator()
    (Vector4<Real> const & a_point, Ray<Real> const & a_line)
  {
    Result result;

    Vector4<Real> w = a_point - a_line.Origin();
    result.u = w.Dot(a_line.Direction());

    if (result.u < static_cast<Real>(0.0))
    {
      result.u = static_cast<Real>(0.0);
    }

    result.cp = a_line.Origin() + result.u * a_line.Direction();
    result.sqDistance = SquaredDistance(result.cp, a_point);
    result.distance = sqrt(result.sqDistance);
    return result;
  }
} //End: DCPQuery::operator()

#endif