//! @file DgQueryLineSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESPHERE_H
#define DGQUERYLINESPHERE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgLine.h"
#include "..\DgSphere.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a sphere.
  template <typename Real>
  class TIQuery<Real, Line<Real>, Sphere<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects sphere.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(Line<Real> const &, Sphere<Real> const &);
  };


  //! @ingroup DgMath_geoQueries
  //! Find the intersection point between a line and a sphere.
  template <typename Real>
  class FIQuery<Real, Line<Real>, Sphere<Real>>
  {
  public:

    //! Query result data
    struct Result
    {
      //! Point of intersection from u0. Not set if line does not intersect sphere.
      Vector4<Real> p0;

      //! Point of intersection from u1. Not set if line does not intersect sphere.
      Vector4<Real> p1;

      //! Distance from the line origin to the first point of intersection. Not set if line does not intersect sphere.
      Real u0;

      //! Distance from the line origin to the second point of intersection. Not set if line does not intersect sphere.
      Real u1  ;

      //! Return code. Codes include:
      //!   - <code><b>0</b></code>: Line intersects Sphere
      //!   - <code><b>1</b></code>: Line does not intersect sphere.
      int code;
    };

    //! Perform query
    Result operator()(Line<Real> const &, Sphere<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TILineSphere = TIQuery<Real, Line<Real>, Sphere<Real>>;

  //! Template alias for convenience
  template<typename Real>
  using FILineSphere = FIQuery<Real, Line<Real>, Sphere<Real>>;


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, Line<Real>, Sphere<Real>>::Result
    TIQuery<Real, Line<Real>, Sphere<Real>>::operator()
    (Line<Real> const & a_line, Sphere<Real> const & a_sphere)
  {
    Result result;
    Vector4<Real> w0(a_line.Origin() - a_sphere.Center());
    Real a = a_line.Direction().LengthSquared();
    Real b = w0.Dot(a_line.Direction());
    Real c = w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius();
    result.isIntersecting = (b*b >= a*c);
    return result;
  } //End: DCPQuery::operator()


  //--------------------------------------------------------------------------------
  //	@	DCPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename FIQuery<Real, Line<Real>, Sphere<Real>>::Result
    FIQuery<Real, Line<Real>, Sphere<Real>>::operator()
    (Line<Real> const & a_line, Sphere<Real> const & a_sphere)
  {
    Result result;
    Vector4<Real> w0(a_line.Origin() - a_sphere.Center());
    Real a = a_line.Direction().LengthSquared();
    Real b = static_cast<Real>(2.0) * w0.Dot(a_line.Direction());
    Real c = w0.LengthSquared() - a_sphere.Radius() * a_sphere.Radius();
    
    Real discr = b*b - static_cast<Real>(4.0)*a*c;
    if (discr < static_cast<Real>(0.0))
    {
      result.code = 1;
    }
    else
    {
      Real d = sqrt(discr);
      Real e = static_cast<Real>(1.0) / (static_cast<Real>(2.0) * a);
      result.u0 = (-b - d) * e;
      result.u1 = (-b + d) * e;
      result.p0 = a_line.Origin() + result.u0 * a_line.Direction();
      result.p1 = a_line.Origin() + result.u1 * a_line.Direction();
      result.code = 0;   
    }

    return result;
  } //End: DCPQuery::operator()
}

#endif