//! @file DgQuerySegmentPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYSEGMENTPLANE_H
#define DGQUERYSEGMENTPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgR3Segment.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line segment and a plane.
  template <typename Real>
  class TIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects plane.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(R3::Segment<Real> const &, R3::Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line segment and a plane.
  template <typename Real>
  class FIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Point of intersection. Set to origin if line segment is parallel to plane.
      R3::Vector4<Real> point;

      //! Distance from the line segment origin to the point of intersection. Set to 0 if line segment is parallel to the plane.
      Real u;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Line segment intersects plane
      //!   - <code><b>1</b></code>: Line segment lies on the plane. Output set to line origin.
      //!   - <code><b>2</b></code>: No Intersection. 
      int code;
    };

    //! Perform query
    Result operator()(R3::Segment<Real> const &, R3::Plane<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TISegmentPlane = TIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>;

  //! Template alias for convenience
  template<typename Real>
  using FISegmentPlane = FIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	TIQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>::Result
    TIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>::operator()
    (R3::Segment<Real> const & a_seg, R3::Plane<Real> const & a_plane)
  {
    Result result;

    R3::Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    R3::Vector4<Real> so(a_seg.Origin());
    R3::Vector4<Real> sd(a_seg.Direction());

    Real denom = pn.Dot(sd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if ray is on the plane
      if (Dg::IsZero(a_plane.SignedDistance(so)))
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
      Real u = (-(so.Dot(pn) + po) / denom);

      if (u < static_cast<Real>(0.0) ||
          u > static_cast<Real>(1.0))
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
  typename FIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>::Result
    FIQuery<Real, R3::Segment<Real>, R3::Plane<Real>>::operator()
    (R3::Segment<Real> const & a_seg, R3::Plane<Real> const & a_plane)
  {
    Result result;

    R3::Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    R3::Vector4<Real> so(a_seg.Origin());
    R3::Vector4<Real> sd(a_seg.Direction());

    Real denom = pn.Dot(sd);

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0.0);

      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(so)))
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
      result.u = (-(so.Dot(pn) + po) / denom);
      if (result.u < static_cast<Real>(0.0))
      {
        result.u = static_cast<Real>(0.0);
        result.code = 2;
      }
      else if (result.u > static_cast<Real>(1.0))
      {
        result.u = static_cast<Real>(1.0);
        result.code = 2;
      }
      else
      {
        result.code = 0;
      }
    }

    result.point = so + result.u * sd;
    return result;
  } //End: FIQuery::operator()
}

#endif