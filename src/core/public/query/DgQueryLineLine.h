//! @file DgQueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINELINE_H
#define DGQUERYLINELINE_H

#include "DgDCPQuery.h"
#include "..\DgLine.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Line, Line
  template <typename Real>
  class DCPQuery<Real, Line<Real>, Line<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      //! Distance between lines
      Real distance;

      //! Squared distance between lines
      Real sqDistance;

      //! Distance from line 0 origin to closest point to line 1
      Real u0;
      
      //! Distance from line 1 origin to closest point to line 0
      Real u1;

      //! Closest point on line 0 to line 1
      Vector4<Real> cp0;
      
      //! Closest point on line 1 to line 0
      Vector4<Real> cp1;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Lines are parallel. Closest points are based off line 0 origin.
      int code;
    };

    //! Perform query.
    Result operator()(Line<Real> const & a_line0, Line<Real> const & a_line1);
  };
  
  //! Template alias for convenience.
  template<typename Real>
  using DCPLineLine = DCPQuery<Real, Line<Real>, Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename DCPQuery<Real, Line<Real>, Line<Real>>::Result
    DCPQuery<Real, Line<Real>, Line<Real>>::operator()
    (Line<Real> const & a_line0, Line<Real> const & a_line1)
  {
    Result result;

    Vector4<Real> o0(a_line0.Origin());
    Vector4<Real> o1(a_line1.Origin());
    Vector4<Real> d0(a_line0.Direction());
    Vector4<Real> d1(a_line1.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d1);
    Real b = d0.Dot(w0);
    Real c = d1.Dot(w0);
    Real d = static_cast<Real>(1.0) - a*a;

    if (Dg::IsZero(d))
    {
      result.u0 = static_cast<Real>(0.0);
      result.u1 = c;
      result.cp0 = o0;
      result.cp1 = o1 + c * d1;
      vec diff = result.cp0 - result.cp1;
      result.sqDistance = diff.LengthSquared();
      result.distance = sqrt(result.sqDistance);
      result.code = 1;
    }
    else
    {
      result.u0 = ((a*c - b) / d);
      result.u1 = ((c - a*b) / d);
      result.cp0 = o0 + result.u0 * d0;
      result.cp1 = o1 + result.u1 * d1;
      vec diff = result.cp0 - result.cp1;
      result.sqDistance = diff.LengthSquared();
      result.distance = sqrt(result.sqDistance);
      result.code = 0;
    }
    return result;
  } //End: DCPQuery::operator()
}

#endif