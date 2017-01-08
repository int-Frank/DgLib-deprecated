//! @file DgQueryRayPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYRAYPLANE_H
#define DGQUERYRAYPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgR3Ray.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a ray and a plane.
  template <typename Real>
  class TIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if ray intersects plane.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(R3::Ray<Real> const &, R3::Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a ray and a plane.
  template <typename Real>
  class FIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Point of intersection. Set to origin if ray is parallel to plane.
      R3::Vector4<Real> point;

      //! Distance from the ray origin to the point of intersection. Set to 0 if ray is parallel to the plane.
      Real u;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: R3::Ray intersects plane
      //!   - <code><b>1</b></code>: R3::Ray lies on the plane. Output set to ray origin.
      //!   - <code><b>2</b></code>: No Intersection. 
      int code;
    };

    //! Perform query
    Result operator()(R3::Ray<Real> const &, R3::Plane<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TIRayPlane = TIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>;

  //! Template alias for convenience
  template<typename Real>
  using FIRayPlane = FIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	TIQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>::Result
    TIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>::operator()
    (R3::Ray<Real> const & a_ray, R3::Plane<Real> const & a_plane)
  {
    Result result;

    R3::Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    R3::Vector4<Real> ro(a_ray.Origin());
    R3::Vector4<Real> rd(a_ray.Direction());

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
  typename FIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>::Result
    FIQuery<Real, R3::Ray<Real>, R3::Plane<Real>>::operator()
    (R3::Ray<Real> const & a_ray, R3::Plane<Real> const & a_plane)
  {
    Result result;

    R3::Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    R3::Vector4<Real> ro(a_ray.Origin());
    R3::Vector4<Real> rd(a_ray.Direction());

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