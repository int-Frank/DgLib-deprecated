//! @file DgQueryPointLine.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYPOINTLINE_H
#define DGQUERYPOINTLINE_H

#include "DgCPQuery.h"
#include "..\DgLine.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Distance and closest-point query: Point, Line.
  template <typename Real>
  class CPQuery<Real, Vector4<Real>, Line<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Distance from line origin to closest point to point
      Real u;

      //! Closest point on the line to the point.
      Vector4<Real> cp;
    };

    //! Perform query
    Result operator()(Vector4<Real> const &, Line<Real> const &);
  };

  //! Template alias for convenience
  template<typename Real>
  using CPPointLine = CPQuery<Real, Vector4<Real>, Line<Real>>;


  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename CPQuery<Real, Vector4<Real>, Line<Real>>::Result
    CPQuery<Real, Vector4<Real>, Line<Real>>::operator()
    (Vector4<Real> const & a_point, Line<Real> const & a_line)
  {
    Result result;

    Vector4<Real> w = a_point - a_line.Origin();
    result.u = w.Dot(a_line.Direction());
    result.cp = a_line.Origin() + result.u * a_line.Direction();
    return result;
  }
} //End: CPQuery::operator()

#endif