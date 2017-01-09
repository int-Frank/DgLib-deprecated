//! @file DgR2QueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYPOINTSEGMENT_H
#define DGR2QUERYPOINTSEGMENT_H

#include "DgR2CPQuery.h"
#include "..\DgR2Segment.h"

namespace Dg
{
  namespace R2
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Segment.
    template <typename Real>
    class CPQuery<Real, Vector3<Real>, Segment<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line segment origin to closest point to point
        Real u;

        //! Closest point on the line segment to the point.
        Vector3<Real> cp;
      };

      //! Perform query
      Result operator()(Vector3<Real> const &, Segment<Real> const &);
    };

    //! Template alias for convenience
    template<typename Real>
    using CPPointSegment = CPQuery<Real, Vector3<Real>, Segment<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Vector3<Real>, Segment<Real>>::Result
      CPQuery<Real, Vector3<Real>, Segment<Real>>::operator()
      (Vector3<Real> const & a_point, Segment<Real> const & a_seg)
    {
      Result result;
      Vector3<Real> w = a_point - a_seg.Origin();

      Real proj = w.Dot(a_seg.Direction());

      if (proj <= static_cast<Real>(0.0))
      {
        result.u = static_cast<Real>(0.0);
      }
      else
      {
        Real vsq = a_seg.LengthSquared();

        if (proj >= vsq)
        {
          result.u = static_cast<Real>(1.0);
        }
        else
        {
          result.u = (proj / vsq);
        }
      }

      result.cp = a_seg.Origin() + result.u * a_seg.Direction();
      return result;
    } //End: CPQuery::operator()
  }
}

#endif