//! @file DgQueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTLINE_H
#define DGQUERYPOINTLINE_H

#include "DgCPQuery.h"
#include "..\DgR3Line.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, R3::Line.
  template <typename Real>
  class CPQuery<Real, R3::Vector4<Real>, R3::Line<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance from line origin to closest point to point
      Real u;

      //! Closest point on the line to the point.
      R3::Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(R3::Vector4<Real> const &, R3::Line<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointLine = CPQuery<Real, R3::Vector4<Real>, R3::Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, R3::Vector4<Real>, R3::Line<Real>>::Result
    CPQuery<Real, R3::Vector4<Real>, R3::Line<Real>>::operator()
    (R3::Vector4<Real> const & a_point, R3::Line<Real> const & a_line)
  {
    Result result;

    R3::Vector4<Real> w = a_point - a_line.Origin();
    result.u = w.Dot(a_line.Direction());
    result.cp = a_line.Origin() + result.u * a_line.Direction();
    return result;
  }
} //End: CPQuery::operator()

#endif