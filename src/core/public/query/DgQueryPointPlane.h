//! @file DgQueryPointPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgDCPQuery.h"
#include "..\DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, Plane.
  template <typename Real>
  class DCPQuery<Real, Vector4<Real>, Plane<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance between point and line
      Real distance;

      //! Signed distance between point and line
      Real sDistance;

      //! Closest point on the plane to the point.
      Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(Vector4<Real> const &, Plane<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using DCPPointPlane = DCPQuery<Real, Vector4<Real>, Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, Vector4<Real>, Plane<Real>>::Result
    DCPQuery<Real, Vector4<Real>, Plane<Real>>::operator()
    (Vector4<Real> const & a_point, Plane<Real> const & a_plane)
  {
    Result result;

    result.sDistance = a_plane.SignedDistance(a_point);
    result.distance = abs(result.sDistance);
    result.cp = a_point - result.sDistance * a_plane.Normal();

    return result;
  }
} //End: DCPQuery::operator()

#endif