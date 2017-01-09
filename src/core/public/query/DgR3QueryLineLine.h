//! @file DgR3QueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYLINELINE_H
#define DGR3QUERYLINELINE_H

#include "DgQueryCommon.h"
#include "DgR3CPQuery.h"
#include "../DgR3Line.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line, Line
    template <typename Real>
    class CPQuery<Real, Line<Real>, Line<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Distance from line 0 origin to closest point to line 1
        Real u0;

        //! Distance from line 1 origin to closest point to line 0
        Real u1;

        //! Closest point on line 0 to line 1
        Vector<Real> cp0;

        //! Closest point on line 1 to line 0
        Vector<Real> cp1;

        //! Return codes include:
        //! Success, Parallel
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Line<Real> const &, Line<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPLineLine = CPQuery<Real, Line<Real>, Line<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Line<Real>, Line<Real>>::Result
      CPQuery<Real, Line<Real>, Line<Real>>::operator()
      (Line<Real> const & a_line0, Line<Real> const & a_line1)
    {
      Result result;

      Vector<Real> o0(a_line0.Origin());
      Vector<Real> o1(a_line1.Origin());
      Vector<Real> d0(a_line0.Direction());
      Vector<Real> d1(a_line1.Direction());

      //compute intermediate parameters
      Vector<Real> w0(o0 - o1);
      Real a = d0.Dot(d1);
      Real b = d0.Dot(w0);
      Real c = d1.Dot(w0);
      Real d = static_cast<Real>(1.0) - a*a;

      if (Dg::IsZero(d))
      {
        result.u0 = static_cast<Real>(0.0);
        result.u1 = c;
        result.code = QueryCode::Parallel;
      }
      else
      {
        result.u0 = ((a*c - b) / d);
        result.u1 = ((c - a*b) / d);
        result.code = QueryCode::Success;
      }

      result.cp0 = o0 + result.u0 * d0;
      result.cp1 = o1 + result.u1 * d1;
      return result;
    } //End: CPQuery::operator()
  }
}

#endif