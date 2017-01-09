//! @file DgR3QueryRayLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYRAYLINE_H
#define DGR3QUERYRAYLINE_H

#include "DgQueryCommon.h"
#include "DgR3CPQuery.h"
#include "..\DgR3Line.h"
#include "..\DgR3Ray.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Ray, Line
    template <typename Real>
    class CPQuery<Real, Ray<Real>, Line<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the line origin to closest point to the ray
        Real ul;

        //! Distance from the ray origin to closest point to the line
        Real ur;

        //! Closest point on line to the ray
        Vector<Real> cpl;

        //! Closest point on ray to the line
        Vector<Real> cpr;

        //! Return code. Codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Ray<Real> const &, Line<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPRayLine = CPQuery<Real, Ray<Real>, Line<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Ray<Real>, Line<Real>>::Result
      CPQuery<Real, Ray<Real>, Line<Real>>::operator()
      (Ray<Real> const & a_ray, Line<Real> const & a_line)
    {
      Vector<Real> or (a_ray.Origin());
      Vector<Real> ol(a_line.Origin());
      Vector<Real> dr(a_ray.Direction());
      Vector<Real> dl(a_line.Direction());

      //compute intermediate parameters
      Vector<Real> diff(or -ol);
      Real a = dr.Dot(dl);
      Real b = dl.Dot(diff);
      Real c = static_cast<Real>(1.0) - a * a;

      Result result;

      //if c is zero, try finding closest point on the ray to the line origin
      if (Dg::IsZero(c))
      {
        //compute closest points
        result.ur = static_cast<Real>(0.0);
        result.ul = b;
        result.code = QueryCode::Parallel;
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

        result.code = Success;
      }

      result.cpr = or +result.ur*dr;
      result.cpl = ol + result.ul*dl;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif