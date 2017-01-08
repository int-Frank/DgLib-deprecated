//! @file DgR3QueryRayPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYRAYPLANE_H
#define DGR3QUERYRAYPLANE_H

#include "DgQueryCommon.h"
#include "DgR3TIQuery.h"
#include "DgR3FIQuery.h"
#include "..\DgR3Ray.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a ray and a plane.
    template <typename Real>
    class TIQuery<Real, Ray<Real>, Plane<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if ray intersects plane.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Ray<Real> const &, Plane<Real> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a ray and a plane.
    template <typename Real>
    class FIQuery<Real, Ray<Real>, Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersection. Set to origin if ray is parallel to plane.
        Vector4<Real> point;

        //! Distance from the ray origin to the point of intersection. Set to 0 if ray is parallel to the plane.
        Real u;

        //! Return code. Codes include:
        //! Intersecting, Overlapping, NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(Ray<Real> const &, Plane<Real> const &);
    };


    //! Template alias for convenience
    template<typename Real>
    using TIRayPlane = TIQuery<Real, Ray<Real>, Plane<Real>>;

    //! Template alias for convenience
    template<typename Real>
    using FIRayPlane = FIQuery<Real, Ray<Real>, Plane<Real>>;


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, Ray<Real>, Plane<Real>>::Result
      TIQuery<Real, Ray<Real>, Plane<Real>>::operator()
      (Ray<Real> const & a_ray, Plane<Real> const & a_plane)
    {
      Result result;

      Vector4<Real> pn(a_plane.Normal());
      Real          po(a_plane.Offset());
      Vector4<Real> ro(a_ray.Origin());
      Vector4<Real> rd(a_ray.Direction());

      Real denom = pn.Dot(rd);

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        //check if ray is on the plane
        if (Dg::IsZero(a_plane.Distance(ro)))
        {
          result.isIntersecting = true;
        }
        else
        {
          result.isIntersecting = false;
        }
      }
      else
      {
        Real u = (-(ro.Dot(pn) + po) / denom);

        //ray points away from plane
        if (u < static_cast<Real>(0.0))
        {
          result.isIntersecting = false;
        }
        else
        {
          result.isIntersecting = true;
        }
      }

      return result;
    } //End: TIQuery::operator()


      //--------------------------------------------------------------------------------
      //	@	FIQuery::operator()
      //--------------------------------------------------------------------------------
    template<typename Real>
    typename FIQuery<Real, Ray<Real>, Plane<Real>>::Result
      FIQuery<Real, Ray<Real>, Plane<Real>>::operator()
      (Ray<Real> const & a_ray, Plane<Real> const & a_plane)
    {
      Result result;

      Vector4<Real> pn(a_plane.Normal());
      Real          po(a_plane.Offset());
      Vector4<Real> ro(a_ray.Origin());
      Vector4<Real> rd(a_ray.Direction());

      Real denom = pn.Dot(rd);

      //check if ray is parallel to plane
      if (Dg::IsZero(denom))
      {
        result.u = static_cast<Real>(0.0);
        result.point = ro;

        //check if ray is on the plane
        if (Dg::IsZero(a_plane.Distance(ro)))
        {
          result.code = QueryCode::Overlapping;
        }
        else
        {
          result.code = QueryCode::NotIntersecting;
        }
      }
      else
      {
        result.u = (-(ro.Dot(pn) + po) / denom);

        //ray points away from plane
        if (result.u < static_cast<Real>(0.0))
        {
          result.u = static_cast<Real>(0.0);
          result.point = ro;
          result.code = QueryCode::NotIntersecting;
        }
        else
        {
          result.point = ro + result.u * rd;
          result.code = QueryCode::Intersecting;
        }
      }

      return result;
    } //End: FIQuery::operator()
  }
}

#endif