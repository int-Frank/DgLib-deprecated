//! @file DgQueryLineLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINELINE_H
#define DGQUERYLINELINE_H

#include "DgCPQuery.h"
#include "../DgR3Line.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: R3::Line, R3::Line
  template <typename Real>
  class CPQuery<Real, R3::Line<Real>, R3::Line<Real>>
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
      R3::Vector4<Real> cp0;
      
      //! Closest point on line 1 to line 0
      R3::Vector4<Real> cp1;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Success
      //!   - <code><b>1</b></code>: Lines are parallel. Closest points are based off line 0 origin.
      int code;
    };

    //! Perform query.
    Result operator()(R3::Line<Real> const &, R3::Line<Real> const &);
  };
  
  //! Template alias for convenience.
  template<typename Real>
  using CPLineLine = CPQuery<Real, R3::Line<Real>, R3::Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, R3::Line<Real>, R3::Line<Real>>::Result
    CPQuery<Real, R3::Line<Real>, R3::Line<Real>>::operator()
    (R3::Line<Real> const & a_line0, R3::Line<Real> const & a_line1)
  {
    Result result;

    R3::Vector4<Real> o0(a_line0.Origin());
    R3::Vector4<Real> o1(a_line1.Origin());
    R3::Vector4<Real> d0(a_line0.Direction());
    R3::Vector4<Real> d1(a_line1.Direction());

    //compute intermediate parameters
    R3::Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d1);
    Real b = d0.Dot(w0);
    Real c = d1.Dot(w0);
    Real d = static_cast<Real>(1.0) - a*a;

    if (Dg::IsZero(d))
    {
      result.u0 = static_cast<Real>(0.0);
      result.u1 = c;
      result.code = 1;
    }
    else
    {
      result.u0 = ((a*c - b) / d);
      result.u1 = ((c - a*b) / d);
      result.code = 0;
    }

    result.cp0 = o0 + result.u0 * d0;
    result.cp1 = o1 + result.u1 * d1;
    return result;
  } //End: CPQuery::operator()
}

#endif