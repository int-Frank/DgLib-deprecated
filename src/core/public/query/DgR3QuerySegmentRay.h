//! @file DgR3QuerySegmentRay.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTRAY_H
#define DGR3QUERYSEGMENTRAY_H

#include "DgQueryCommon.h"
#include "DgR3CPQuery.h"
#include "..\DgR3Ray.h"
#include "..\DgR3Segment.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line Segment, Ray
    template <typename Real>
    class CPQuery<Real, Segment<Real>, Ray<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the ray origin to closest point to the line segment
        Real ur;

        //! Distance from the line segment origin to closest point to the ray
        Real us;

        //! Closest point on ray to the line segment
        Vector4<Real> cpr;

        //! Closest point on line segment to the ray
        Vector4<Real> cps;

        //! Return code. Codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment<Real> const &, Ray<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPSegmentRay = CPQuery<Real, Segment<Real>, Ray<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Segment<Real>, Ray<Real>>::Result
      CPQuery<Real, Segment<Real>, Ray<Real>>::operator()
      (Segment<Real> const & a_seg, Ray<Real> const & a_ray)
    {
      Result result;
      result.code = QueryCode::Success;

      Vector4<Real> os(a_seg.Origin());
      Vector4<Real> or (a_ray.Origin());
      Vector4<Real> ds(a_seg.Direction());
      Vector4<Real> dr(a_ray.Direction());

      //compute intermediate parameters
      Vector4<Real> w0(os - or );
      Real a = ds.Dot(ds);
      Real b = ds.Dot(dr);
      Real c = ds.Dot(w0);
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
        Vector4<Real> w1((os + ds) - or );
        if (!(d < static_cast<Real>(0.0) &&
          w1.Dot(dr) < static_cast<Real>(0.0)))
        {
          result.code = QueryCode::Overlapping;
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
          result.us = static_cast<Real>(0.0);
        }
        // clamp result.uls to 1
        else if (-c > a)
        {
          result.us = static_cast<Real>(1.0);
        }
        else
        {
          result.us = -c / a;
        }
      }
      else
      {
        result.ur = tn / td;
        result.us = sn / sd;
      }

      result.cps = os + result.us*ds;
      result.cpr = or +result.ur*dr;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif