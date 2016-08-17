//! @file DgQueryLineSegmentSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGQUERYLINESEGMENTSPHERE_H
#define DGQUERYLINESEGMENTSPHERE_H

#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgLineSegment.h"
#include "..\DgSphere.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //! Test for intersection between a line and a sphere.
  template <typename Real>
  class TIQuery<Real, LineSegment<Real>, Sphere<Real>>
  {
  public:

    //! Query result data.
    struct Result
    {
      //! True if line intersects sphere.
      bool isIntersecting;
    };

    //! Perform query.
    Result operator()(LineSegment<Real> const &, Sphere<Real> const &);
  };


  //! Template alias for convenience
  template<typename Real>
  using TILineSegmentSphere = TIQuery<Real, LineSegment<Real>, Sphere<Real>>;

  //--------------------------------------------------------------------------------
  //	@	CPQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, LineSegment<Real>, Sphere<Real>>::Result
    TIQuery<Real, LineSegment<Real>, Sphere<Real>>::operator()
    (LineSegment<Real> const & a_ls, Sphere<Real> const & a_sphere)
  {
    Result result;
    Vector4<Real> w0(a_ls.Origin() - a_sphere.Center());
    Real a = a_ls.Direction().LengthSquared();
    Real inv_a = static_cast<Real>(1.0) / a;
    Real b = inv_a * w0.Dot(a_ls.Direction());
    Real c = inv_a * (w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius());
    Real discriminant = b * b + c;

    if (discriminant < static_cast<Real>(0.0))
    {
      result.isIntersecting = false;
      return result;
    }

    //TODO Not complete
    Real irr = sqrt(b * b - static_cast<Real>(4.0) * a * c);
    Real u0 = (-b - irr) / (static_cast<Real>(2.0) * a);
    Real u1 = (-b + irr) / (static_cast<Real>(2.0) * a);
    result.isIntersecting = (u0 > static_cast<Real>(0.0) && u0 < static_cast<Real>(1.0))
      || (u1 > static_cast<Real>(0.0) && u1 < static_cast<Real>(1.0))
      || (u0 * u1 < static_cast<Real>(0.0))
    return result;
  } //End: CPQuery::operator()

}

#endif