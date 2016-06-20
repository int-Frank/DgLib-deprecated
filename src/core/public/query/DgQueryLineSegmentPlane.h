//! @file DgQueryLineSewgmentPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESEGMENTPLANE_H
#define DGQUERYLINESEGMENTPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgLineSegment.h"
#include "..\DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a plane.
  template <typename Real>
  class TIQuery<Real, LineSegment<Real>, Plane<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects plane.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(LineSegment<Real> const &, Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a plane.
  template <typename Real>
  class FIQuery<Real, LineSegment<Real>, Plane<Real>>
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
      //!   - <code><b>0</b></code>: Line segment intersects plane
      //!   - <code><b>1</b></code>: Line segment lies on the plane. Output set to line origin.
      //!   - <code><b>2</b></code>: No Intersection. 
      int code;
    };

    //! Perform query
    Result operator()(LineSegment<Real> const &, Plane<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TILineSegmentPlane = TIQuery<Real, LineSegment<Real>, Plane<Real>>;

  //! Template alias for convenience
  template<typename Real>
  using FILineSegmentPlane = FIQuery<Real, LineSegment<Real>, Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	TIQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, LineSegment<Real>, Plane<Real>>::Result
    TIQuery<Real, LineSegment<Real>, Plane<Real>>::operator()
    (LineSegment<Real> const & a_ls, Plane<Real> const & a_plane)
  {
    Result result;

    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> lso(a_ls.Origin());
    Vector4<Real> lsd(a_ls.Direction());

    Real denom = pn.Dot(lsd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if ray is on the plane
      if (Dg::IsZero(a_plane.SignedDistance(lso)))
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
      Real u = (-(lso.Dot(pn) + po) / denom);

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
  typename FIQuery<Real, LineSegment<Real>, Plane<Real>>::Result
    FIQuery<Real, LineSegment<Real>, Plane<Real>>::operator()
    (LineSegment<Real> const & a_ls, Plane<Real> const & a_plane)
  {
    Result result;

    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> lso(a_ls.Origin());
    Vector4<Real> lsd(a_ls.Direction());

    Real denom = pn.Dot(lsd);

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0.0);

      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(lso)))
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
      result.u = (-(lso.Dot(pn) + po) / denom);
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

    result.point = lso + result.u * lsd;
    return result;
  } //End: FIQuery::operator()
}

#endif