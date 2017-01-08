//! @file DgR3QuerySegmentLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTLINE_H
#define DGR3QUERYSEGMENTLINE_H

#include "DgR3CPQuery.h"
#include "..\DgR3Line.h"
#include "..\DgR3Segment.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line segment, Line
    template <typename Real>
    class CPQuery<Real, Segment<Real>, Line<Real>>
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
        Vector4<Real> cpl;

        //! Closest point on line segment to the line
        Vector4<Real> cps;

        //! Return code. Codes include:
        //!   - <code><b>0</b></code>: Success
        //!   - <code><b>1</b></code>: Line segment and line are parallel.
        int code;
      };

      //! Perform query.
      Result operator()(Segment<Real> const &, Line<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPSegmentLine = CPQuery<Real, Segment<Real>, Line<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Segment<Real>, Line<Real>>::Result
      CPQuery<Real, Segment<Real>, Line<Real>>::operator()
      (Segment<Real> const & a_seg, Line<Real> const & a_line)
    {
      Result result;

      Vector4<Real> os(a_seg.Origin());
      Vector4<Real> ol(a_line.Origin());
      Vector4<Real> ds(a_seg.Direction());
      Vector4<Real> dl(a_line.Direction());

      //compute intermediate parameters
      Vector4<Real> w0(os - ol);
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
        result.code = 1;
      }
      else
      {
        result.code = 0;

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