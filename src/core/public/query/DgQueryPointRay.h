//! @file DgQueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTRAY_H
#define DGQUERYPOINTRAY_H

#include "DgCPQuery.h"
#include "..\DgR3Ray.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, R3::Ray.
  template <typename Real>
  class CPQuery<Real, R3::Vector4<Real>, R3::Ray<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance from ray origin to closest point to point
      Real u;

      //! Closest point on the ray to the point.
      R3::Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(R3::Vector4<Real> const &, R3::Ray<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointRay = CPQuery<Real, R3::Vector4<Real>, R3::Ray<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, R3::Vector4<Real>, R3::Ray<Real>>::Result
    CPQuery<Real, R3::Vector4<Real>, R3::Ray<Real>>::operator()
    (R3::Vector4<Real> const & a_point, R3::Ray<Real> const & a_line)
  {
    Result result;

    R3::Vector4<Real> w = a_point - a_line.Origin();
    result.u = w.Dot(a_line.Direction());

    if (result.u < static_cast<Real>(0.0))
    {
      result.u = static_cast<Real>(0.0);
    }

    result.cp = a_line.Origin() + result.u * a_line.Direction();
    return result;
  }
} //End: CPQuery::operator()

#endif