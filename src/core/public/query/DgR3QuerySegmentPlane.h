//! @file DgR3QuerySegmentPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTPLANE_H
#define DGR3QUERYSEGMENTPLANE_H

#include "DgQueryCommon.h"
#include "DgR3TIQuery.h"
#include "DgR3FIQuery.h"
#include "..\DgR3Segment.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line segment and a plane.
    template <typename Real>
    class TIQuery<Real, Segment<Real>, Plane<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects plane.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Segment<Real> const &, Plane<Real> const &);
    };


    //! @ingroup DgMath_geoQueries
    //! Find the intersection point between a line segment and a plane.
    template <typename Real>
    class FIQuery<Real, Segment<Real>, Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Point of intersection. Set to origin if line segment is parallel to plane.
        Vector4<Real> point;

        //! Distance from the line segment origin to the point of intersection. Set to 0 if line segment is parallel to the plane.
        Real u;

        //! Return code. Codes include:
        //! Intersecting, Overlapping, NotIntersecting
        QueryCode code;
      };

      //! Perform query
      Result operator()(Segment<Real> const &, Plane<Real> const &);
    };


    //! Template alias for convenience
    template<typename Real>
    using TISegmentPlane = TIQuery<Real, Segment<Real>, Plane<Real>>;

    //! Template alias for convenience
    template<typename Real>
    using FISegmentPlane = FIQuery<Real, Segment<Real>, Plane<Real>>;


    //--------------------------------------------------------------------------------
    //	@	TIQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, Segment<Real>, Plane<Real>>::Result
      TIQuery<Real, Segment<Real>, Plane<Real>>::operator()
      (Segment<Real> const & a_seg, Plane<Real> const & a_plane)
    {
      Result result;

      Vector4<Real> pn(a_plane.Normal());
      Real          po(a_plane.Offset());
      Vector4<Real> so(a_seg.Origin());
      Vector4<Real> sd(a_seg.Direction());

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
    typename FIQuery<Real, Segment<Real>, Plane<Real>>::Result
      FIQuery<Real, Segment<Real>, Plane<Real>>::operator()
      (Segment<Real> const & a_seg, Plane<Real> const & a_plane)
    {
      Result result;

      Vector4<Real> pn(a_plane.Normal());
      Real          po(a_plane.Offset());
      Vector4<Real> so(a_seg.Origin());
      Vector4<Real> sd(a_seg.Direction());

      Real denom = pn.Dot(sd);

      //check if line is parallel to plane
      if (Dg::IsZero(denom))
      {
        result.u = static_cast<Real>(0.0);

        //check if line is on the plane
        if (Dg::IsZero(a_plane.Distance(so)))
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
        result.u = (-(so.Dot(pn) + po) / denom);
        if (result.u < static_cast<Real>(0.0))
        {
          result.u = static_cast<Real>(0.0);
          result.code = QueryCode::NotIntersecting;
        }
        else if (result.u > static_cast<Real>(1.0))
        {
          result.u = static_cast<Real>(1.0);
          result.code = QueryCode::NotIntersecting;
        }
        else
        {
          result.code = QueryCode::Intersecting;
        }
      }

      result.point = so + result.u * sd;
      return result;
    } //End: FIQuery::operator()
  }
}

#endif