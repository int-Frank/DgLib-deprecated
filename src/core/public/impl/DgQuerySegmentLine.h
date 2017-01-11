//! @file DgQuerySegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYSEGMENTLINE_H
#define DGQUERYSEGMENTLINE_H

#include "../query/DgQueryCommon.h"
#include "DgCPQuery.h"
#include "DgLine_generic.h"
#include "DgSegment_generic.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line segment, Line
    template <typename Real, int R>
    class CPQuery<Real, R,
                  Segment_generic<Real, R>, 
                  Line_generic<Real, R>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from the line origin to closest point to the line segment
        Real ul;

        //! Distance from the line segment origin to closest point to the line
        Real us;

        //! Closest point on line to the line segment
        Vector_generic<Real, R> cpl;

        //! Closest point on line segment to the line
        Vector_generic<Real, R> cps;

        //! Return code. Codes include:
        //! Success, Parallel.
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment_generic<Real, R> const &, Line_generic<Real, R> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    typename CPQuery<Real, R, Segment_generic<Real, R>, Line_generic<Real, R>>::Result
      CPQuery<Real, R, Segment_generic<Real, R>, Line_generic<Real, R>>::operator()
      (Segment_generic<Real, R> const & a_seg, Line_generic<Real, R> const & a_line)
    {
      Result result;

      Vector_generic<Real, R> os(a_seg.Origin());
      Vector_generic<Real, R> ol(a_line.Origin());
      Vector_generic<Real, R> ds(a_seg.Direction());
      Vector_generic<Real, R> dl(a_line.Direction());

      //compute intermediate parameters
      Vector_generic<Real, R> w0(os - ol);
      Real a = ds.Dot(ds);
      Real b = ds.Dot(dl);
      Real c = ds.Dot(w0);
      Real d = dl.Dot(w0);
      Real denom = a - b*b;

      // if denom is zero, try finding closest point on line to segment origin
      if (Dg::IsZero(denom))
      {
        result.us = static_cast<Real>(0.0);
        result.ul = d;
        result.code = QueryCode::Parallel;
      }
      else
      {
        result.code = QueryCode::Success;

        // clamp result.uls within [0,1]
        Real sn = b*d - c;

        // clamp result.uls to 0
        if (sn < static_cast<Real>(0.0))
        {
          result.us = static_cast<Real>(0.0);
          result.ul = d;
        }
        // clamp result.uls to 1
        else if (sn > denom)
        {
          result.us = static_cast<Real>(1.0);
          result.ul = (d + b);
        }
        else
        {
          result.us = sn / denom;
          result.ul = (a*d - b*c) / denom;
        }
      }

      result.cps = os + result.us*ds;
      result.cpl = ol + result.ul*dl;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif