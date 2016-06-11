//! @file DgQueryLinePlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINEPLANE_H
#define DGQUERYLINEPLANE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgLine.h"
#include "..\DgPlane.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a plane.
  template <typename Real>
  class TIQuery<Real, Line<Real>, Plane<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects plane.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(Line<Real> const &, Plane<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a plane.
  template <typename Real>
  class FIQuery<Real, Line<Real>, Plane<Real>>
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
      //!   - <code><b>0</b></code>: Line intersects plane
      //!   - <code><b>1</b></code>: Line lies on the plane. Output set to line origin.
      //!   - <code><b>2</b></code>: No Intersection. Line is parllel to the plane. Output point not set.
      int code;
    };

    //! Perform query
    Result operator()(Line<Real> const &, Plane<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TILinePlane = TIQuery<Real, Line<Real>, Plane<Real>>;

  //! Template alias for convenience
  template<typename Real>
  using FILinePlane = FIQuery<Real, Line<Real>, Plane<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, Line<Real>, Plane<Real>>::Result
    TIQuery<Real, Line<Real>, Plane<Real>>::operator()
    (Line<Real> const & a_line, Plane<Real> const & a_plane)
  {
    Result result;
    result.isIntersecting = !Dg::IsZero(a_plane.NormalDot(a_line.Direction())) ||
                             Dg::IsZero(a_plane.SignedDistance(a_line.Origin()));
    return result;
  } //End: DCPQuery::operator()


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename FIQuery<Real, Line<Real>, Plane<Real>>::Result
    FIQuery<Real, Line<Real>, Plane<Real>>::operator()
    (Line<Real> const & a_line, Plane<Real> const & a_plane)
  {
    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> lo(a_line.Origin());
    Vector4<Real> ld(a_line.Direction());

    Real denom = pn.Dot(ld);
    Result result;

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      result.u = static_cast<Real>(0.0);
      result.point = lo;

      //check if line is on the plane
      result.code = Dg::IsZero(a_plane.Distance(lo)) ? 1 : 2;
    }
    else
    {
      result.u = -(lo.Dot(pn) + po) / denom;
      result.point = lo + result.u * ld;
      result.code = 0;
    }

    return result;
  } //End: DCPQuery::operator()
}

#endif