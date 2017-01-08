//! @file DgQueryPointPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTPLANE_H
#define DGQUERYPOINTPLANE_H

#include "DgCPQuery.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, R3::Plane.
  template <typename Real>
  class CPQuery<Real, R3::Vector4<Real>, R3::Plane<Real>>
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
      R3::Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(R3::Vector4<Real> const &, R3::Plane<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointPlane = CPQuery<Real, R3::Vector4<Real>, R3::Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, R3::Vector4<Real>, R3::Plane<Real>>::Result
    CPQuery<Real, R3::Vector4<Real>, R3::Plane<Real>>::operator()
    (R3::Vector4<Real> const & a_point, R3::Plane<Real> const & a_plane)
  {
    Result result;

    result.sDistance = a_plane.SignedDistance(a_point);
    result.distance = abs(result.sDistance);
    result.cp = a_point - result.sDistance * a_plane.Normal();

    return result;
  }
} //End: CPQuery::operator()

#endif