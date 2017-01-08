//! @file DgR3QueryRayRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYRAYRAY_H
#define DGR3QUERYRAYRAY_H

#include "DgQueryCommon.h"
#include "DgR3CPQuery.h"
#include "..\DgR3Ray.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Ray, Ray
    template <typename Real>
    class CPQuery<Real, Ray<Real>, Ray<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from ray 0 origin to closest point to ray 1
        Real u0;

        //! Distance from ray 1 origin to closest point to ray 0
        Real u1;

        //! Closest point on ray 0 to ray 1
        Vector4<Real> cp0;

        //! Closest point on ray 1 to ray 0
        Vector4<Real> cp1;

        //! Return code. Codes include:
        //! Success, Overlapping
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Ray<Real> const &, Ray<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPRayRay = CPQuery<Real, Ray<Real>, Ray<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Ray<Real>, Ray<Real>>::Result
      CPQuery<Real, Ray<Real>, Ray<Real>>::operator()
      (Ray<Real> const & a_ray0, Ray<Real> const & a_ray1)
    {
      Result result;
      result.code = QueryCode::Success;

      Vector4<Real> o0(a_ray0.Origin());
      Vector4<Real> o1(a_ray1.Origin());
      Vector4<Real> d0(a_ray0.Direction());
      Vector4<Real> d1(a_ray1.Direction());

      //compute intermediate parameters
      Vector4<Real> w0(o0 - o1);
      Real a = d0.Dot(d1);
      Real b = d0.Dot(w0);
      Real c = d1.Dot(w0);
      Real d = static_cast<Real>(1.0) - a * a;

      //parameters to compute result.u0 and result.u1
      Real sn, sd, tn, td;

      //if denom is zero, try finding closest point on ray1 to origin of ray0
      if (Dg::IsZero(d))
      {
        //clamp result.u0 to 0
        sd = td = static_cast<Real>(1.0);
        sn = static_cast<Real>(0.0);
        tn = c;

        //Do the rays overlap?
        if (!(c < static_cast<Real>(0.0) &&
          b > static_cast<Real>(0.0)))
        {
          result.code = QueryCode::Overlapping;
        }
      }
      else
      {
        //clamp result.u0 within[0, +inf]
        sd = td = d;
        sn = a*c - b;
        tn = c - a*b;

        //clamp result.u0 to 0
        if (sn < static_cast<Real>(0.0))
        {
          sn = static_cast<Real>(0.0);
          tn = c;
          td = static_cast<Real>(1.0);
        }
      }

      //clamp result.u1 within [0, +inf]
      //clamp result.u1 to 0
      if (tn < static_cast<Real>(0.0))
      {
        result.u1 = static_cast<Real>(0.0);

        //clamp result.u0 to 0
        if (-b < static_cast<Real>(0.0))
        {
          result.u0 = static_cast<Real>(0.0);
        }
        else
        {
          result.u0 = -b;
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
}

#endif