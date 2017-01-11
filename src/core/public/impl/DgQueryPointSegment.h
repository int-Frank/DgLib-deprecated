//! @file DgQueryPointSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTSEGMENT_H
#define DGQUERYPOINTSEGMENT_H

#include "DgCPQuery.h"
#include "..\query\DgQueryCommon.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Segment.
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Vector_generic<Real, R>,
                  Segment_generic<Real, R>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance from line segment origin to closest point to point
        Real u;

        //! Closest point on the line segment to the point.
        Vector_generic<Real, R> cp;
      };

      //! Perform query
      Result operator()(Vector_generic<Real, R> const &, Segment_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::Result
      CPQuery<Real, R, Vector_generic<Real, R>, Segment_generic<Real, R>>::operator()
      (Vector_generic<Real, R> const & a_point, Segment_generic<Real, R> const & a_seg)
    {
      Result result;
      Vector_generic<Real, R> w = a_point - a_seg.Origin();

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