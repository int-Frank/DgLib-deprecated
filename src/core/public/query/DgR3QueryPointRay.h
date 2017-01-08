//! @file DgR3QueryPointRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTRAY_H
#define DGR3QUERYPOINTRAY_H

#include "DgR3CPQuery.h"
#include "..\DgR3Ray.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Ray.
    template <typename Real>
    class CPQuery<Real, Vector4<Real>, Ray<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
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
    using CPPointRay = CPQuery<Real, Vector4<Real>, Ray<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Vector4<Real>, Ray<Real>>::Result
      CPQuery<Real, Vector4<Real>, Ray<Real>>::operator()
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
      return result;
    } //End: CPQuery::operator()
  }
}

#endif