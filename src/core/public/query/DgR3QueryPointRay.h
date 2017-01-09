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
    class CPQuery<Real, Vector<Real>, Ray<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from ray origin to closest point to point
        Real u;

        //! Closest point on the ray to the point.
        Vector<Real> cp;
      };

      //! Perform query
      Result operator()(Vector<Real> const &, Ray<Real> const &);
    };

    //! Template alias for convenience
    template<typename Real>
    using CPPointRay = CPQuery<Real, Vector<Real>, Ray<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Vector<Real>, Ray<Real>>::Result
      CPQuery<Real, Vector<Real>, Ray<Real>>::operator()
      (Vector<Real> const & a_point, Ray<Real> const & a_line)
    {
      Result result;

      Vector<Real> w = a_point - a_line.Origin();
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