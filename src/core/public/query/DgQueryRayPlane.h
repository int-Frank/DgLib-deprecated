//! @file DgQueryRayPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYRAYPLANE_H
#define DGQUERYRAYPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgRay.h"
#include "..\DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a plane.
  template <typename Real>
  class TIQuery<Real, Ray<Real>, Plane<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects plane.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(Ray<Real> const &, Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a plane.
  template <typename Real>
  class FIQuery<Real, Ray<Real>, Plane<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Point of intersection. Set to origin if line is parallel to plane.
      Vector4<Real> point;

      //! Distance from the line origin to the point of intersection. Set to 0 if line is parallel to the plane.
      Real u;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Ray intersects plane
      //!   - <code><b>1</b></code>: Ray lies on the plane. Output set to line origin.
      //!   - <code><b>2</b></code>: No Intersection. 
      int code;
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
        result.code = 1;
      }
      else
      {
        result.code = 2;
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
        result.code = 2;
      }
      else
      {
        result.point = ro + result.u * rd;
        result.code = 0;
      }
    }

    return result;
  } //End: FIQuery::operator()
}

#endif